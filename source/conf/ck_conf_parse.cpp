/*
    Copyright (c) 2007-2010 Takashi Kitao

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/


#include "ck_conf_all.h"

#include "ck_math_all.h"
#include "ck_low_level_api.h"


struct Value
{
    Value* next;
    ckEnt::ValueType type;

    union
    {
        s32 val_s32;
        r32 val_r32;
        char* val_str;
    };
};


class Parser
{
public:
    Parser(const void* src, u32 src_size)
    {
        m_cursor = static_cast<const char*>(src);
        m_remain_size = src_size;
        m_line_no = 1;
        m_is_err = false;

        skipSpace();
    }

    bool isEnd() const
    {
        return (m_remain_size == 0 || m_is_err);
    }

    bool isError() const
    {
        return m_is_err;
    }

    u16 getErrorLineNo()
    {
        return isError() ? m_line_no : 0;
    }

    void parseChar(char c)
    {
        if (isError())
        {
            return;
        }

        skipSpace();

        if (getNext() == c)
        {
            moveForward();
        }
        else
        {
            m_is_err = true;
        }
    }

    bool tryToParseChar(char c)
    {
        if (isError())
        {
            return false;
        }

        skipSpace();

        if (getNext() == c)
        {
            moveForward();

            return true;
        }
        else
        {
            return false;
        }
    }

    void parseIdentifier(ckStr<char, ckEnt::MAX_NAME_LENGTH>* name)
    {
        if (isError())
        {
            return;
        }

        skipSpace();

        *name = "";

        while (!isEnd())
        {
            char c = getNext();

            if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')
            {
                if (name->getLength() >= ckEnt::MAX_NAME_LENGTH)
                {
                    m_is_err = true;
                }

                *name += c;

                moveForward();
            }
            else
            {
                return;
            }
        }
    }

    void parseNumber(Value* val)
    {
        val->type = ckEnt::TYPE_S32;
        val->val_s32 = 0;

        if (isError())
        {
            return;
        }

        skipSpace();

        bool is_minus = false;
        char c = getNext();

        if (c == '-')
        {
            is_minus = true;

            moveForward();

            c = getNext();
        }

        if (c >= '0' && c <= '9')
        {
            val->val_s32 = c - '0';

            moveForward();

            for (c = getNext(); c >= '0' && c <= '9'; c = getNext())
            {
                val->val_s32 *= 10;
                val->val_s32 += c - '0';

                moveForward();
            }

            if (c == '.')
            {
                val->type = ckEnt::TYPE_R32;
                val->val_r32 = static_cast<r32>(val->val_s32);

                moveForward();

                r32 decimal = 0.1f;

                for (c = getNext(); c >= '0' && c <= '9'; c = getNext())
                {
                    val->val_r32 += (c - '0') * decimal;
                    decimal *= 0.1f;

                    moveForward();
                }

                if (is_minus)
                {
                    val->val_r32 = -val->val_r32;
                }
            }
            else
            {
                if (is_minus)
                {
                    val->val_s32 = -val->val_s32;
                }
            }
        }
    }

    u16 parseString1()
    {
        if (isError())
        {
            return 0;
        }

        u16 len = 0;
        const char* cursor = m_cursor;

        for (u16 remain_size = m_remain_size; remain_size > 0; remain_size--)
        {
            if (*cursor == '"')
            {
                return len;
            }
            else if (*cursor == '\n' || *cursor == '\r')
            {
                break;
            }
            else if (*cursor == '\\')
            {
                if (remain_size == 0)
                {
                    break;
                }

                cursor++;
                remain_size--;
            }

            len++;
            cursor++;
        }

        m_is_err = true;
        return 0;
    }

    void parseString2(Value* val, char* str_buf, u16 str_buf_size)
    {
        val->type = ckEnt::TYPE_STRING;
        val->val_str = str_buf;

        if (isError())
        {
            return;
        }

        while (!isEnd())
        {
            char c = getNext();
            moveForward();

            if (c == '"')
            {
                if (str_buf_size == 0)
                {
                    break;
                }

                *str_buf = '\0';

                return;
            }
            else if (c == '\n' || c == '\r')
            {
                break;
            }
            else if (c == '\\')
            {
                if (isEnd())
                {
                    break;
                }

                c = getNext();
                moveForward();

                if (c == 'n')
                {
                    c = '\n';
                }
                else if (c == 't')
                {
                    c = '\t';
                }
            }

            if (str_buf_size > 0)
            {
                *str_buf = c;

                str_buf++;
                str_buf_size--;
            }
            else
            {
                break;
            }
        }

        m_is_err = true;
    }

    void checkEnd()
    {
        skipSpace();
    }

private:
    char getNext() const
    {
        if (isError())
        {
            return '\0';
        }

        return (m_remain_size > 0) ? *m_cursor : '\0';
    }

    void skipSpace()
    {
        if (isError())
        {
            return;
        }

        bool is_end_of_line = false;
        char end_of_line_char = '\0';

        while (!isEnd())
        {
            char c = getNext();

            if (c == '#')
            {
                moveForward();

                while (!isEnd())
                {
                    c = getNext();

                    if (c == '\n' || c == '\r')
                    {
                        break;
                    }

                    moveForward();
                }

                continue;
            }

            if (c == '\n' || c == '\r')
            {
                if (!is_end_of_line)
                {
                    is_end_of_line = true;
                    end_of_line_char = c;

                    m_line_no++;
                }
                else if (end_of_line_char != c)
                {
                    is_end_of_line = false;
                }

                moveForward();
            }
            else
            {
                is_end_of_line = false;

                if (c == ' ' || c == '\t')
                {
                    moveForward();
                }
                else
                {
                    return;
                }
            }
        }
    }

    void moveForward()
    {
        if (isError())
        {
            return;
        }

        if (m_remain_size > 0)
        {
            m_cursor++;
            m_remain_size--;
        }
    }

    const char* m_cursor;
    u32 m_remain_size;
    u16 m_line_no;
    bool m_is_err;
};


void ckConf::parse(const void* data, u32 data_size)
{
    Parser pars(data, data_size);

    while (!pars.isEnd())
    {
        ckStr<char, ckEnt::MAX_NAME_LENGTH> ent_name;
        pars.parseIdentifier(&ent_name);

        pars.parseChar('=');

        if (pars.tryToParseChar('('))
        {
            u16 val_num = 0;
            Value* rev_val_list = NULL;

            while (!pars.isEnd())
            {
                Value* val = static_cast<Value*>(m_malloc(sizeof(Value)));

                if (pars.tryToParseChar('"'))
                {
                    u16 str_buf_size = pars.parseString1() + 1;
                    char* str_buf = static_cast<char*>(m_malloc(str_buf_size));

                    pars.parseString2(val, str_buf, str_buf_size);
                }
                else
                {
                    pars.parseNumber(val);
                }

                val->next = rev_val_list;
                rev_val_list = val;

                val_num++;

                if (pars.tryToParseChar(')'))
                {
                    break;
                }

                pars.parseChar(',');
            }

            Value* val_list = NULL;
            Value* val_next;

            for (Value* val = rev_val_list; val; val = val_next)
            {
                val_next = val->next;

                val->next = val_list;
                val_list = val;
            }

            ckEnt* ent = newEntry(ent_name.getString(), val_num);
            u16 val_index = 0;

            for (Value* val = val_list; val; val = val->next)
            {
                ent->m_val_type[val_index] = val->type;

                switch (val->type)
                {
                case ckEnt::TYPE_S32:
                    ent->m_val[val_index].val_s32 = val->val_s32;
                    break;

                case ckEnt::TYPE_R32:
                    ent->m_val[val_index].val_r32 = val->val_r32;
                    break;

                case ckEnt::TYPE_STRING:
                    ent->m_val[val_index].val_str = val->val_str;
                    break;
                }

                val_index++;
            }

            for (Value* val = val_list; val; val = val_next)
            {
                val_next = val->next;

                m_free(val);
            }
        }
        else
        {
            Value val;

            if (pars.tryToParseChar('"'))
            {
                u16 str_buf_size = pars.parseString1() + 1;
                char* str_buf = static_cast<char*>(m_malloc(str_buf_size));

                pars.parseString2(&val, str_buf, str_buf_size);
            }
            else
            {
                pars.parseNumber(&val);
            }

            ckEnt* ent = newEntry(ent_name.getString(), 1);

            ent->m_val_type[0] = val.type;

            switch (val.type)
            {
            case ckEnt::TYPE_S32:
                ent->m_val[0].val_s32 = val.val_s32;
                break;

            case ckEnt::TYPE_R32:
                ent->m_val[0].val_r32 = val.val_r32;
                break;

            case ckEnt::TYPE_STRING:
                ent->m_val[0].val_str = val.val_str;
                break;
            }
        }

        pars.checkEnd();
    }

    if (pars.isError() && isValid())
    {
        m_err_line_no = pars.getErrorLineNo();

        ckLowLevelAPI::printf("*** config file compile error ***\n");
    }
}

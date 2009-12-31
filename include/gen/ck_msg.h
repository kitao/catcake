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


/*!
    @ingroup ckGen
    A general purpose message with fixed number parameters.
    @tparam N The number of parameters. This value must be more than zero.
*/
template<u8 N> class ckMsg
{
public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionInvalidType);
    //! @endcond

    /*!
        Constructs a ckMsg.
    */
    ckMsg()
    {
        for (s32 i = 0; i < N; i++)
        {
            m_param_size[i] = 0;
        }
    }

    /*!
        Returns the parameter of the specified index.
        @tparam T The type of the parameter.
        @param[in] index The index of a parameter.
        @return The parameter of the specified index.
    */
    template<class T> T getParam(u8 index)
    {
        if (index >= N)
        {
            ckThrow(ExceptionInvalidArgument);
        }

        if (m_param_size[index] == 0)
        {
            ckThrow(ExceptionInvalidCall);
        }

        if (sizeof(T) != m_param_size[index])
        {
            ckThrow(ExceptionInvalidType);
        }

        return *reinterpret_cast<T*>(&m_param[index]);
    }

    /*!
        Sets the parameter to the specified value.
        @tparam T The type of the parameter.
        @param[in] index The index of a parameter.
        @param[in] param The value of the parameter.
    */
    template<class T> void setParam(u8 index, T param)
    {
        if (index >= N)
        {
            ckThrow(ExceptionInvalidArgument);
        }

        if (sizeof(T) > sizeof(MsgParam))
        {
            ckThrow(ExceptionInvalidType);
        }

        *reinterpret_cast<T*>(&m_param[index]) = param;
        m_param_size[index] = sizeof(T);
    }

private:
    struct MsgParam
    {
        u8 data[8];
    };

    MsgParam m_param[N];
    u8 m_param_size[N];
};


//! @cond
template<> class ckMsg<0>
{
private:
    ckMsg() {}
};
//! @endcond

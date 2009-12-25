/*
    Copyright (c) 2007-2009 Takashi Kitao
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
    `  notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
    `  notice, this list of conditions and the following disclaimer in the
    `  documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products
    `  derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*!
    @ingroup ckGen
    A general purpose message with fixed number parameters.
    @tparam N The number of parameters. This value must be more than zero.
*/
template<u8 N> class ckMsg
{
public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionInvalidType);

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


template<> class ckMsg<0>
{
private:
    ckMsg() {}
};

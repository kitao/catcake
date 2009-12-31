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


class ckResMgr;


/*!
    @ingroup ckRes
    A resource, which contains data.
*/
class CK_API ckRes
{
    friend class ckResMgr;

public:
    /*!
        Constructs a resource.
    */
    ckRes();

    /*!
        Returns the ID of this resource.
        @return The ID of this resource.
    */
    ckID getID() const;

    /*!
        Returns the extension of this resource.
        @return The extension of this resource.
    */
    ckStr<char, 3> getExtension() const;

    /*!
        Returns the data of this resource.
        @tparam T The data type.
        @return The data of this resource.
    */
    template<class T> const T* getData() const
    {
        return reinterpret_cast<const T*>(m_data);
    }

    /*!
        Returns the size of the data
        @return The size of the data
    */
    u32 getDataSize() const;

    /*!
        Returns the extra information of this resource.
        @tparam T Tha type of the extra information.
        @return The extra information of this resource.
    */
    template<class T> T* getExInfo() const
    {
        return reinterpret_cast<T*>(m_exinfo);
    }

    /*!
        Returns whether releases the data automatically.
        @return Whether releases the data automatically.
    */
    bool isAutoFree() const;

private:
    void init(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void* exinfo, bool is_auto_free);

    static ckStr<char, 3> toUppercase(ckStr<char, 3> str);

    ckID m_id;
    ckStr<char, 3> m_ext;
    const void* m_data;
    u32 m_data_size;
    void* m_exinfo;
    bool m_is_auto_free;
};

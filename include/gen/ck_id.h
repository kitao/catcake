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
    @ingroup pgGen
    A unique value to identify objects.
*/
class PG_API pgID
{
public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionOutOfID);

    static const pgID ZERO; //!< The initial id.

    /*!
        Constructs and initializes a pgID to pgID::ZERO.
    */
    pgID();

    /*!
        Returns whether this pgID is equal to the right hand side pgID.
        @param[in] id The right hand side pgID.
        @return Whether this pgID is equal to the right hand side pgID.
    */
    bool operator==(pgID id) const;

    /*!
        Returns whether this pgID is unequal to the right hand side pgID.
        @param[in] id The right hand side pgID.
        @return Whether this pgID is unequal to the right hand side pgID.
    */
    bool operator!=(pgID id) const;

    /*!
        Returns the remainder of the value of this pgID devided by the specified value.
        This method is used when pgID is used as the key type of pgMap.
        @param[in] n A value to devide the value of pgID.
        @return The remainder of the value of this pgID devided by the specified value.
    */
    u32 operator%(u32 n) const;

    /*!
        Returns the value of this pgID.
        @return The value of this pgID.
    */
    u32 getValue() const;

    /*!
        Generates a unique pgID.
        @return A unique pgID.
    */
    static pgID genID();

    /*!
        Generates a pgID whose value is generated from the specified string.
        There may be rare occations when the same value is generated from different strings.
        @param[in] str The seed of a unique value.
        @return A pgID.
    */
    static pgID genID(const char* str);

    /*!
        Generates a pgID from the specified value. This method is only for system.
        @param[in] value The value of the pgID to be generated.
        @return A pgID.
    */
    static pgID genIDForSystem(u32 value);

    /*!
        Sets the value, which is used to generate the next pgID, to the specified value. This method is only for system.
        @param[in] value A value which is used to generate the next pgID.
    */
    static void setCurIDForSystem(u32 value);

private:
    static const u32 BIT_NUM = 32;
    static const u32 MAX_ID = (static_cast<u64>(1) << (BIT_NUM - 1)) - 1;

    u32 m_id;

    static u32 m_cur_id;
};


/*!
    Generates a pgID whose value is generated from the specified string literal.
    There may be rare occations when the same value is generated from different strings.
    @param[in] str The seed of a unique value.
    @return A pgID.
*/
#define pgID_(str) pgID::genIDForSystem(PG_ID_00(str, sizeof("" str)))


#define PG_ID_00(str, len) ((len <= 1) ? 0 : (PG_ID_01(str, len - 1) | 0x80000000))
#define PG_ID_01(str, len) ((len <= 1) ? str[0] : PG_ID_02(str, len - 1) * 37 + str[len - 1])
#define PG_ID_02(str, len) ((len <= 1) ? str[0] : PG_ID_03(str, len - 1) * 37 + str[len - 1])
#define PG_ID_03(str, len) ((len <= 1) ? str[0] : PG_ID_04(str, len - 1) * 37 + str[len - 1])
#define PG_ID_04(str, len) ((len <= 1) ? str[0] : PG_ID_05(str, len - 1) * 37 + str[len - 1])
#define PG_ID_05(str, len) ((len <= 1) ? str[0] : PG_ID_06(str, len - 1) * 37 + str[len - 1])
#define PG_ID_06(str, len) ((len <= 1) ? str[0] : PG_ID_07(str, len - 1) * 37 + str[len - 1])
#define PG_ID_07(str, len) ((len <= 1) ? str[0] : PG_ID_08(str, len - 1) * 37 + str[len - 1])
#define PG_ID_08(str, len) ((len <= 1) ? str[0] : PG_ID_09(str, len - 1) * 37 + str[len - 1])
#define PG_ID_09(str, len) ((len <= 1) ? str[0] : PG_ID_10(str, len - 1) * 37 + str[len - 1])
#define PG_ID_10(str, len) ((len <= 1) ? str[0] : PG_ID_11(str, len - 1) * 37 + str[len - 1])
#define PG_ID_11(str, len) ((len <= 1) ? str[0] : PG_ID_12(str, len - 1) * 37 + str[len - 1])
#define PG_ID_12(str, len) ((len <= 1) ? str[0] : PG_ID_13(str, len - 1) * 37 + str[len - 1])
#define PG_ID_13(str, len) ((len <= 1) ? str[0] : PG_ID_14(str, len - 1) * 37 + str[len - 1])
#define PG_ID_14(str, len) ((len <= 1) ? str[0] : PG_ID_15(str, len - 1) * 37 + str[len - 1])
#define PG_ID_15(str, len) ((len <= 1) ? str[0] : PG_ID_16(str, len - 1) * 37 + str[len - 1])
#define PG_ID_16(str, len) ((len <= 1) ? str[0] : PG_ID_17(str, len - 1) * 37 + str[len - 1])
#define PG_ID_17(str, len) ((len <= 1) ? str[0] : PG_ID_18(str, len - 1) * 37 + str[len - 1])
#define PG_ID_18(str, len) ((len <= 1) ? str[0] : PG_ID_19(str, len - 1) * 37 + str[len - 1])
#define PG_ID_19(str, len) ((len <= 1) ? str[0] : PG_ID_20(str, len - 1) * 37 + str[len - 1])
#define PG_ID_20(str, len) ((len <= 1) ? str[0] : PG_ID_21(str, len - 1) * 37 + str[len - 1])
#define PG_ID_21(str, len) ((len <= 1) ? str[0] : PG_ID_22(str, len - 1) * 37 + str[len - 1])
#define PG_ID_22(str, len) ((len <= 1) ? str[0] : PG_ID_23(str, len - 1) * 37 + str[len - 1])
#define PG_ID_23(str, len) ((len <= 1) ? str[0] : PG_ID_24(str, len - 1) * 37 + str[len - 1])
#define PG_ID_24(str, len) ((len <= 1) ? str[0] : PG_ID_25(str, len - 1) * 37 + str[len - 1])
#define PG_ID_25(str, len) ((len <= 1) ? str[0] : PG_ID_26(str, len - 1) * 37 + str[len - 1])
#define PG_ID_26(str, len) ((len <= 1) ? str[0] : PG_ID_27(str, len - 1) * 37 + str[len - 1])
#define PG_ID_27(str, len) ((len <= 1) ? str[0] : PG_ID_28(str, len - 1) * 37 + str[len - 1])
#define PG_ID_28(str, len) ((len <= 1) ? str[0] : PG_ID_29(str, len - 1) * 37 + str[len - 1])
#define PG_ID_29(str, len) ((len <= 1) ? str[0] : PG_ID_30(str, len - 1) * 37 + str[len - 1])
#define PG_ID_30(str, len) ((len <= 1) ? str[0] : PG_ID_31(str, len - 1) * 37 + str[len - 1])
#define PG_ID_31(str, len) ((len <= 1) ? str[0] : PG_ID_32(str, len - 1) * 37 + str[len - 1])
#define PG_ID_32(str, len) ((len <= 1) ? str[0] : 0)

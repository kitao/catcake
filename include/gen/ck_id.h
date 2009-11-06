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
    A unique value to identify objects.
*/
class CK_API ckID
{
public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionOutOfID);

    static const ckID ZERO; //!< The initial id.
    static const u8 MAX_STRING_LENGTH = 32; //!< The maximum length of a string used for generating ckID.

    /*!
        Constructs and initializes a ckID to ckID::ZERO.
    */
    ckID();

    /*!
        Returns whether this ckID is equal to the right hand side ckID.
        @param[in] id The right hand side ckID.
        @return Whether this ckID is equal to the right hand side ckID.
    */
    bool operator==(ckID id) const;

    /*!
        Returns whether this ckID is unequal to the right hand side ckID.
        @param[in] id The right hand side ckID.
        @return Whether this ckID is unequal to the right hand side ckID.
    */
    bool operator!=(ckID id) const;

    /*!
        Returns the remainder of the value of this ckID devided by the specified value.
        This method is used when ckID is used as the key type of ckMap.
        @param[in] n A value to devide the value of ckID.
        @return The remainder of the value of this ckID devided by the specified value.
    */
    u32 operator%(u32 n) const;

    /*!
        Returns the value of this ckID.
        @return The value of this ckID.
    */
    u32 getValue() const;

    /*!
        Generates a unique ckID.
        @return A unique ckID.
    */
    static ckID genID();

    /*!
        Generates a ckID whose value is generated from the specified string.
        There may be rare occations when the same value is generated from different strings.
        @param[in] str The seed of a unique value.
        @return A ckID.
    */
    static ckID genID(const char* str);

    /*!
        Generates a ckID from the specified value. This method is only for system.
        @param[in] value The value of the ckID to be generated.
        @return A ckID.
    */
    static ckID genIDForSystem(u32 value);

    /*!
        Sets the value, which is used to generate the next ckID, to the specified value. This method is only for system.
        @param[in] value A value which is used to generate the next ckID.
    */
    static void setCurIDForSystem(u32 value);

private:
    static const u32 BIT_NUM = 32;
    static const u32 MAX_ID = (static_cast<u64>(1) << (BIT_NUM - 1)) - 1;

    u32 m_id;

    static u32 m_cur_id;
};


/*!
    Generates a ckID whose value is generated from the specified string literal.
    There may be rare occations when the same value is generated from different strings.
    @param[in] str The seed of a unique value.
    @return A ckID.
*/
#define ckID_(str) ckID::genIDForSystem(CK_ID_00(str, (sizeof("" str) < (ckID::MAX_STRING_LENGTH + 1) ? sizeof(str) : (ckID::MAX_STRING_LENGTH + 1))))


#define CK_ID_00(str, len) ((len <= 1) ? 0 : (CK_ID_01(str, len - 1) | 0x80000000))
#define CK_ID_01(str, len) ((len <= 1) ? str[0] : CK_ID_02(str, len - 1) * 37 + str[len - 1])
#define CK_ID_02(str, len) ((len <= 1) ? str[0] : CK_ID_03(str, len - 1) * 37 + str[len - 1])
#define CK_ID_03(str, len) ((len <= 1) ? str[0] : CK_ID_04(str, len - 1) * 37 + str[len - 1])
#define CK_ID_04(str, len) ((len <= 1) ? str[0] : CK_ID_05(str, len - 1) * 37 + str[len - 1])
#define CK_ID_05(str, len) ((len <= 1) ? str[0] : CK_ID_06(str, len - 1) * 37 + str[len - 1])
#define CK_ID_06(str, len) ((len <= 1) ? str[0] : CK_ID_07(str, len - 1) * 37 + str[len - 1])
#define CK_ID_07(str, len) ((len <= 1) ? str[0] : CK_ID_08(str, len - 1) * 37 + str[len - 1])
#define CK_ID_08(str, len) ((len <= 1) ? str[0] : CK_ID_09(str, len - 1) * 37 + str[len - 1])
#define CK_ID_09(str, len) ((len <= 1) ? str[0] : CK_ID_10(str, len - 1) * 37 + str[len - 1])
#define CK_ID_10(str, len) ((len <= 1) ? str[0] : CK_ID_11(str, len - 1) * 37 + str[len - 1])
#define CK_ID_11(str, len) ((len <= 1) ? str[0] : CK_ID_12(str, len - 1) * 37 + str[len - 1])
#define CK_ID_12(str, len) ((len <= 1) ? str[0] : CK_ID_13(str, len - 1) * 37 + str[len - 1])
#define CK_ID_13(str, len) ((len <= 1) ? str[0] : CK_ID_14(str, len - 1) * 37 + str[len - 1])
#define CK_ID_14(str, len) ((len <= 1) ? str[0] : CK_ID_15(str, len - 1) * 37 + str[len - 1])
#define CK_ID_15(str, len) ((len <= 1) ? str[0] : CK_ID_16(str, len - 1) * 37 + str[len - 1])
#define CK_ID_16(str, len) ((len <= 1) ? str[0] : CK_ID_17(str, len - 1) * 37 + str[len - 1])
#define CK_ID_17(str, len) ((len <= 1) ? str[0] : CK_ID_18(str, len - 1) * 37 + str[len - 1])
#define CK_ID_18(str, len) ((len <= 1) ? str[0] : CK_ID_19(str, len - 1) * 37 + str[len - 1])
#define CK_ID_19(str, len) ((len <= 1) ? str[0] : CK_ID_20(str, len - 1) * 37 + str[len - 1])
#define CK_ID_20(str, len) ((len <= 1) ? str[0] : CK_ID_21(str, len - 1) * 37 + str[len - 1])
#define CK_ID_21(str, len) ((len <= 1) ? str[0] : CK_ID_22(str, len - 1) * 37 + str[len - 1])
#define CK_ID_22(str, len) ((len <= 1) ? str[0] : CK_ID_23(str, len - 1) * 37 + str[len - 1])
#define CK_ID_23(str, len) ((len <= 1) ? str[0] : CK_ID_24(str, len - 1) * 37 + str[len - 1])
#define CK_ID_24(str, len) ((len <= 1) ? str[0] : CK_ID_25(str, len - 1) * 37 + str[len - 1])
#define CK_ID_25(str, len) ((len <= 1) ? str[0] : CK_ID_26(str, len - 1) * 37 + str[len - 1])
#define CK_ID_26(str, len) ((len <= 1) ? str[0] : CK_ID_27(str, len - 1) * 37 + str[len - 1])
#define CK_ID_27(str, len) ((len <= 1) ? str[0] : CK_ID_28(str, len - 1) * 37 + str[len - 1])
#define CK_ID_28(str, len) ((len <= 1) ? str[0] : CK_ID_29(str, len - 1) * 37 + str[len - 1])
#define CK_ID_29(str, len) ((len <= 1) ? str[0] : CK_ID_30(str, len - 1) * 37 + str[len - 1])
#define CK_ID_30(str, len) ((len <= 1) ? str[0] : CK_ID_31(str, len - 1) * 37 + str[len - 1])
#define CK_ID_31(str, len) ((len <= 1) ? str[0] : CK_ID_32(str, len - 1) * 37 + str[len - 1])
#define CK_ID_32(str, len) ((len <= 1) ? str[0] : 0)

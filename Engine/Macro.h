#pragma once


#define DEVICE				GEngine->GetDevice()->GetDevice()
#define CMD_LIST			GEngine->GetCmdQueue()->GetCmdList()
#define ROOT_SIGNATURE		GEngine->GetRootSignature()->GetSignature()
#define RESOURCE_CMD_LIST	GEngine->GetCmdQueue()->GetResourceCmdList()
#define CONST_BUFFER(type)	GEngine->GetConstantBuffer(type)

#define DELTATIME			GET_SINGLE(Timer)->GetDeltaTime()
#define INPUT				GET_SINGLE(Input)
#define GUI					GEngine->GetGUI()

#define BASE_PATH           "BasePath"
#define TEXTRUE_PATH        "Texture"
#define SHADER_PATH         "Shader"

#define DECLARE_SINGLE(type)		\
private:							\
	type() {}						\
	~type() {}						\
public:								\
	static type* GetI()				\
	{								\
		static type instance;		\
		return &instance;			\
	}								\

#define GET_SINGLE(type)		type::GetI()

#define WEAK(T) T.lock()

/* ------ 제공 : Utils Forever ------ */
// magic_enum도 있긴 있지만 이해한 코드를 쓰는게 좋을 것 같아서 사용합니다.
// https://github.com/utilForever/CubbyEnums/blob/master/Fruit.hpp

enum class LAYER_TYPE : uint32
{
#define X(a) a,
#include "layer.def"
#undef X
};

const std::string LAYER_TYPE_STR[] = {
#define X(a) #a,
#include "layer.def"
#undef X
};

template <class T>
T StrToEnum(std::string);
template <class T>
std::string EnumToStr(T);

#define STR2ENUM(TYPE, ARRAY)                                                \
    template <>                                                              \
    inline TYPE StrToEnum<TYPE>(std::string str)                             \
    {                                                                        \
        for (std::size_t i = 0; i < (sizeof(ARRAY) / sizeof(ARRAY[0])); ++i) \
        {                                                                    \
            if (ARRAY[i] == str)                                             \
            {                                                                \
                return TYPE(i);                                              \
            }                                                                \
        }                                                                    \
                                                                             \
        return TYPE(0);                                                      \
    }

#define ENUM2STR(TYPE, ARRAY)                   \
    template <>                                 \
    inline std::string EnumToStr<TYPE>(TYPE v)  \
    {                                           \
        return ARRAY[static_cast<int>(v)];      \
    }

#define ENUM_AND_STR(TYPE, ARRAY)               \
    STR2ENUM(TYPE, ARRAY)                       \
    ENUM2STR(TYPE, ARRAY)

ENUM_AND_STR(LAYER_TYPE, LAYER_TYPE_STR)
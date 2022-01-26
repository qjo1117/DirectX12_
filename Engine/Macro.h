#pragma once


#define DEVICE				        GEngine->GetDevice()->GetDevice()
#define GRAPHICS_CMD_LIST	        GEngine->GetGraphicsCmdQueue()->GetGraphicsCmdList()
#define RESOURCE_CMD_LIST	        GEngine->GetGraphicsCmdQueue()->GetResourceCmdList()
#define COMPUTE_CMD_LIST	        GEngine->GetComputeCmdQueue()->GetComputeCmdList()

#define GRAPHICS_ROOT_SIGNATURE		GEngine->GetRootSignature()->GetGraphicsSignature()
#define COMPUTE_ROOT_SIGNATURE		GEngine->GetRootSignature()->GetComputeSignature()

#define CONST_BUFFER(type)	        GEngine->GetConstantBuffer(type)

#define DELTATIME			        GET_SINGLE(Timer)->GetDeltaTime()
#define INPUT				        GET_SINGLE(Input)
#define GUI					        GEngine->GetGUI()

#define BASE_PATH_KEY               "BasePath"
#define TEXTURE_PATH_KEY            "Texture"
#define SHADER_PATH_KEY             "Shader"

#define TEXTURE_PATH                GET_SINGLE(PathManager)->FindPath(TEXTURE_PATH_KEY)
#define SHADER_PATH                 GET_SINGLE(PathManager)->FindPath(TEXTURE_PATH_KEY)

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


/* ----- ±èÄ¡»ï ±³¼ö´Ô²²¼­ ¾Ë·ÁÁÖ½Å PROPERTY ----- */
#define PUBLIC_PROPERTY(type, member)                            \
public:                                                             \
type m_##member;                                                    \
public:                                                             \
    void Set##member(const type& p_val) { m_##member = p_val; }     \
    const type& Get##member() { return m_##member; }              

/* ------ Á¦°ø : Utils Forever ------ */
// https://github.com/utilForever/CubbyEnums/blob/master/Fruit.hpp

enum class LAYER_TYPE : uint32
{
#define X(a) a,
#include "layer.hpp"
#undef X
};

const std::string LAYER_TYPE_STR[] = {
#define X(a) #a,
#include "layer.hpp"
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
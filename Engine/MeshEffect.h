#pragma once
#include "Component.h"

class Effect : public Component 
{ 
public:
    typedef struct tagEffect
    {
        const char* pszTag;
        
        string  strVfxMesh;

        // Textures
        pair<_int, string>   pTexture[7] = { {(_int)TextureMapType::END, "None"} };


    }DESC;

public:
    Effect(shared_ptr<Shader> shader);
    virtual ~Effect();

public:
    virtual HRESULT Init(void* pArg);
    virtual void    Tick() override;
    virtual void    Final_Tick() override;  // CS
    void            Render();               // VS, PS

private:

private:
    DESC                    m_tDesc;

    /* Component */
    shared_ptr<Shader>      m_pShader = { nullptr };
    shared_ptr<Mesh>        m_pMesh = { nullptr };
    shared_ptr<Model>       m_pModel = { nullptr };
    shared_ptr<Material>    m_pMaterial = { nullptr };   

};


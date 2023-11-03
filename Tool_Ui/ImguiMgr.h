#pragma once

class ImguiMgr
{
   DECLARE_SINGLETON(ImguiMgr)

public:
   ~ImguiMgr();

public:
   HRESULT     Initialize(HWND& hWnd);
   void        Tick();
   HRESULT     Render();

private:
   void        Key_Input();
   void        Create_Ui();
   void        Manapulate_Ui();


   void        File_Dialog();             // 파일탐색기로 dds추가
   void        Select_Texture();          // 텍스처 선택
   void        Decide_Transform_Value();  // 사이즈, 위치 값 설정
   void        Create_Object();           // 
   

   void        Select_Object();           // 생성한 오브젝트 중 선택
   void        Delete_Object();           // 선택한 오브젝트 삭제
   void        Change_Value_Object();     // 선택한 오브젝트 값 변경

   void        Change_ParamDesc();        // 선택한 오브젝트 파라메터 값 변경
   void        Add_Picking_Zone();

   void        Save_Ui_Desc();            // 
   void        Load_Ui_Desc();            // 





private:
   _bool             m_bRender_Create_Ui     = { false };   // Create tool on/off
   _bool             m_bRender_Manapulate_Ui = { false };   // Manapulate tool on/off

   wstring           m_strSearchTexture;                    // 검색
   wstring           m_strKeyTexture;                       // 선택한 텍스처 키값
   _float3           m_vecPos                = {};          // 생성할 위치
   _float2           m_vecScale              = {};          // 텍스처 크기
   _uint             m_iTextureCursor        = { 0 };       // list box 선택 인덱스

   wstring           m_strName;                             // 생성할 오브젝트 이름
   list<wstring>     m_GameobjectName;                      // 생성한 오브젝트들 이름
   wstring           m_strSelectObjName;                    // 선택한 오브젝트 이름
   _uint             m_iObjNameCursor        = { 0 };       // list box 선택 인덱스

   _bool             m_bIsChangeCheck        = { false };   // 값 변경 즉시 적용

   _int              m_iPass_0               = { 0 };       // 
   _float            m_float_0               = { 0.f };     // 

   _int              m_iPickTypeIndex        = { 0 };       // 
   POINT             m_ptPos1                = {};
   POINT             m_ptPos2                = {};
};


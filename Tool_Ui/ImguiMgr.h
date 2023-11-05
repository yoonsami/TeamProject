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
   void        Parameter_Ui();


   void        File_Dialog();             // 파일탐색기로 dds추가
   void        Select_Texture();          // 텍스처 선택
   void        Add_Texture();
   void        Decide_Transform_Value();  // 사이즈, 위치 값 설정
   void        Create_Object();           // 
   

   void        Select_Object();           // 생성한 오브젝트 중 선택
   void        Delete_Object();           // 선택한 오브젝트 삭제
   void        Change_Object_Value();     // 선택한 오브젝트 값 변경

   void        Change_ParamDesc();        // 선택한 오브젝트 파라메터 값 변경
   void        Add_Picking_Zone();

   void        Save_Ui_Desc();            // 함수 호출시 아래 두 함수도 같이 호출
   void        Decide_FilePath();         // 저장 경로 불러오기
   void        Load_Ui_Desc();            // 

   void        Record_Two_Point();        // 동작 시 이후 두번의 클릭 위치를 저장





private:
   _bool             m_bRender_Create_Ui     = { true };    // Create tool on/off
   _bool             m_bRender_Manapulate_Ui = { true };    // Manapulate tool on/off
   _bool             m_bRender_Param_Ui      = { true };    // Parameter tool on/off

   wstring           m_strSearchTexture;                    // 검색
   wstring           m_strKeyTexture;                       // 선택한 텍스처 키값
   _float3           m_vecPos                = {};          // 생성할 위치
   _float3           m_vecScale              = {};          // 텍스처 크기
   _uint             m_iTextureCursor        = { 0 };       // list box 선택 인덱스
   _int              m_iTextureType          = { 0 };

   wstring           m_strName;                             // 생성할 오브젝트 이름
   list<wstring>     m_GameobjectName;                      // 생성한 오브젝트들 이름
   wstring           m_strSelectObjName;                    // 선택한 오브젝트 이름
   _uint             m_iObjNameCursor        = { 0 };       // list box 선택 인덱스

   shared_ptr<GameObject> m_pSampleObj;
   _bool             m_bIsChangeCheck        = { false };   // 값 변경 즉시 적용
   _int              m_iTypeZmoOper          = { 0 };       // OPERATION 값 변경 라디오 버튼
   OPERATION         m_eZmoOper              = { OPERATION::TRANSLATE };

   _int              m_iPass                 = { 0 };       // Pass Index
   RenderParams      m_tagParamDesc          = {};          // Param Desc

   _int              m_iParam1               = { 0 };
   _int              m_iParam2               = { 0 };
   _int              m_iParam3               = { 0 };
   _int              m_iParam4               = { 0 };

   _int              m_iPickTypeIndex        = { 0 };       // add picking zone 라디오 버튼
   POINT             m_ptPos1                = {};          // 첫 피킹 마우스 좌표
   POINT             m_ptPos2                = {};          // 두번째 피킹 마우스 좌표
   _bool             m_bIsRecordStart        = { false };   // record 버튼 작동확인
   _uint             m_iRecordOrder          = { 0 };       // record 횟수 확인

   wstring           m_strSaveLoadPath;                     // dat 파일 경로
   wstring           m_strSaveFileName;

   const char* m_arrItems[16] = {
      "DIFFUSE",
      "NORMAL",
      "SPECULAR",
      "OPACITY",
      "EMISSIVE",
      "DISSOLVE",
      "DISTORTION",
      "TEXTURE7",
      "TEXTURE8",
      "TEXTURE9",
      "TEXTURE10",
      "TEXTURE11",
      "TEXTURE12",
      "TEXTURE13",
      "TEXTURE14",
      "TEXTURE15",
   };
};


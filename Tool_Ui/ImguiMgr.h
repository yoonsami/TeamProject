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
   void        Font_Ui();

   void        File_Dialog();             // ����Ž����� dds�߰�
   void        Select_Texture();          // �ؽ�ó ����
   void        Add_Texture();             // ������ ������Ʈ�� texture �߰�
   void        Decide_Transform_Value();  // ������, ��ġ �� ����
   void        Create_Object();           // ������Ʈ ����
   

   void        Select_Object();           // ������ ������Ʈ �� ����
   void        Delete_Object();           // ������ ������Ʈ ����
   void        Change_Object_Value();     // ������ ������Ʈ �� ����

   void        Change_ParamDesc();        // ������ ������Ʈ �Ķ���� �� ����
   void        Add_Picking_Zone();        // ��ư �������

   void        Save_Ui_Desc();            // �Լ� ȣ��� �Ʒ� �� �Լ��� ���� ȣ��
   void        Decide_FilePath();         // 
   void        Load_Ui_Desc();            // 

   void        Record_Two_Point();        // ���� �� ���� �ι��� Ŭ�� ��ġ�� ����

   void        Input_Words();             // ���� �Է�
   void        Select_Font_Value();       // ��Ʈ �̸�, ��, ������ ����
   void        Set_Font();                // ��Ʈ ����

private:
   _bool             m_bRender_Create_Ui     = { true };    // Create tool on/off
   _bool             m_bRender_Manapulate_Ui = { true };    // Manapulate tool on/off
   _bool             m_bRender_Param_Ui      = { true };    // Parameter tool on/off
   _bool             m_bRender_Font_Ui       = { true };    // Font Tool on/off

   wstring           m_strSearchTexture;                    // �˻�
   wstring           m_strKeyTexture;                       // ������ �ؽ�ó Ű��
   _float3           m_vecPos                = {};          // ������ ��ġ
   _float3           m_vecScale              = {};          // �ؽ�ó ũ��
   _uint             m_iTextureCursor        = { 0 };       // list box ���� �ε���
   _int              m_iTextureType          = { 0 };

   wstring           m_strName;                             // ������ ������Ʈ �̸�
   list<wstring>     m_GameobjectName;                      // ������ ������Ʈ�� �̸�
   wstring           m_strSelectObjName;                    // ������ ������Ʈ �̸�
   _uint             m_iObjNameCursor        = { 0 };       // list box ���� �ε���

   shared_ptr<GameObject> m_pSampleObj;
   _bool             m_bIsChangeCheck        = { false };   // �� ���� ��� ����
   _int              m_iTypeZmoOper          = { 0 };       // OPERATION �� ���� ���� ��ư
   OPERATION         m_eZmoOper              = { OPERATION::TRANSLATE };

   _int              m_iPass                 = { 0 };       // Pass Index
   RenderParams      m_tagParamDesc          = {};          // Param Desc

   _int              m_iParam1               = { 0 };       // imgui���� int����ؼ� 
   _int              m_iParam2               = { 0 };       // �Ķ���ͷ� �Ѱ��� ����
   _int              m_iParam3               = { 0 };       // ������ ����
   _int              m_iParam4               = { 0 };       //

   _int              m_iPickTypeIndex        = { 0 };       // add picking zone ���� ��ư
   POINT             m_ptPos1                = {};          // ù ��ŷ ���콺 ��ǥ
   POINT             m_ptPos2                = {};          // �ι�° ��ŷ ���콺 ��ǥ
   _bool             m_bIsRecordStart        = { false };   // record ��ư �۵�Ȯ��
   _uint             m_iRecordOrder          = { 0 };       // record Ƚ�� Ȯ��

   wstring           m_strSaveLoadPath;                     // dat ���� ���
   wstring           m_strSaveFileName;                     // dat ���� �̸�

   string            m_strFont;
   wstring           m_wstrFont;
   _int              m_iFontType             = { 0 };       // Font Type
   vector<wstring>   m_vecFontType;                         // ��Ʈ �߰��� �Ʒ� �迭�� init���� �߰��ؾ���

   _float            m_arrColors[4]          = { 0.f, 0.f, 0.f, 0.f };
   _float            m_fFontSize             = { 0.f };

   _int              m_iSetStaticValue       = { 0 };

   const char* m_arrTextureType[16] = {
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

   const char* m_arrFontType[4] = {
      "136ex",
      "Ori_136ex",
      "overwatch",
      "Soyo_Maple"
   };

};


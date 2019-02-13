#include <wx/wx.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>
#include <list>
#include <fstream>
#include <regex>

//储存学生数据
class student
{
public:
    //学号
    std::string ID;
    //姓名
    std::string name;
};

const int ID_Read = 101;
const int ID_Radom = 102;
const int ID_Input = 103;
const int ID_Save = 104;

//应用
class RandStu : public wxApp
{
public:
    //初始化
    virtual bool OnInit();

    //退出
    void OnExit(wxCommandEvent& event)
    {
        GetTopWindow()->Close();
    }

    void OnRead(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnRadom(wxCommandEvent& event);
    void OnInput(wxCommandEvent& event);

private:
    //程序框架
    wxFrame* p_frame;
    //面板
    wxPanel* p_panel;
    //学生列表
    std::list<student> list_student;
    //
    int isRead = 0;
};

IMPLEMENT_APP(RandStu);

bool RandStu::OnInit()
{
    //创建窗口使其在其他应用上层
    p_frame = new wxFrame(NULL, -1, wxT("随机点名系统"));

    //创建面板
    p_panel = new wxPanel(p_frame, wxID_ANY, wxDefaultPosition, wxSize(500, 300));

    //创建按钮
    new wxButton(p_panel, ID_Read, wxT("读取名单"), wxPoint(40, 20), wxDefaultSize);
    new wxButton(p_panel, ID_Radom, wxT("随机点名"), wxPoint(200, 20), wxDefaultSize);
    new wxButton(p_panel, wxID_EXIT, wxT("退出"), wxPoint(120, 120), wxDefaultSize);
    new wxButton(p_panel, ID_Input, wxT("添加"), wxPoint(40, 50), wxDefaultSize);
    new wxButton(p_panel, ID_Save, wxT("保存"), wxPoint(200, 50), wxDefaultSize);

    //链接按钮与事件
    Connect(ID_Read, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RandStu::OnRead));
    Connect(ID_Radom, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RandStu::OnRadom));
    Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RandStu::OnExit));
    Connect(ID_Input, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RandStu::OnInput));
    Connect(ID_Save, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RandStu::OnSave));

    //显示窗口
    GetTopWindow()->Show();

    //进入消息循环
    return true;
};

void RandStu::OnRead(wxCommandEvent& event)
{
    if(isRead == 0) {
        std::ifstream file("Student.txt");
        if(!file) {
            std::cerr << "Can't open file!!!" << std::endl;
            //消息框
            wxMessageBox(wxT("无法读取文件！"), wxT("错误"), wxOK | wxICON_ERROR, GetTopWindow());
            exit(EXIT_FAILURE);
        }
        student TempStu;
        while(file >> TempStu.ID && file >> TempStu.name) {
            list_student.push_back(TempStu);
        }
        wxMessageBox(wxT("读取完毕"), wxT("读取"), wxOK | wxICON_INFORMATION, GetTopWindow());
        isRead = 1;
        file.close();
    }
}

void RandStu::OnRadom(wxCommandEvent& event)
{
    if(list_student.size() == 0)
        return;
    //生成随机数
    srand(time(0));
    int random = rand() % list_student.size();
    //到达随机数所指定的链表位置
    auto pos = list_student.begin();
    for(int i = 0; i != random; i++) {
        pos++;
    }
    //显示人名
    wxMessageBox(wxString::Format(wxT("点名:%s    %s"), pos->ID.c_str(), pos->name.c_str()), wxT("点名"),
        wxOK | wxICON_INFORMATION, GetTopWindow());
}

void RandStu::OnSave(wxCommandEvent& event)
{
    std::ofstream file("Student.txt");
    for(const auto& item : list_student) {
        file << item.ID << ' ' << item.name << std::endl;
    }    
    file.close();
    wxMessageBox(wxT("保存成功"), wxT("保存"), wxOK | wxICON_INFORMATION, GetTopWindow());
}

void RandStu::OnInput(wxCommandEvent& event)
{
    wxTextEntryDialog dialog(GetTopWindow(),wxT("请输入学号和姓名，中间用空格隔开"),
        wxT("输入"), wxT("1160300901 孙月晴"), wxOK | wxCANCEL);
    if(dialog.ShowModal() == wxID_OK)
    {
        //创建正则表达式
        std::regex reg("[^[:space:]]+");
        std::string data(dialog.GetValue());
        //分割字符串
        std::sregex_token_iterator pos(data.cbegin(),data.cend(),reg,{0,2});
        std::sregex_token_iterator end;
        //临时储存信息
        student TempStu;
        TempStu.ID=pos->str();
        pos++;
        pos++;
        TempStu.name=(std::string)(pos->str());
        //追加到链表尾部
        list_student.push_back(TempStu);
        wxMessageBox(wxString::Format(wxT("学号：%s  姓名：%s"),TempStu.ID,TempStu.name), wxT("输入"));
    }
}
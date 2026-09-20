#include <string>

using namespace std;

struct Student{
    private: 
        string name; //name
        float gpa_4; //GPA hệ 4
        float gpa_10;//GPA hệ 10
        bool gender; //true = nam, false = nữ
        string dateOfBirth; // string d/month/year sepparator bằng dấu "/"
        string major; //Tên ngành
        string MSSV; //MSSV, "aabbbccc". a = niên khóa, b = ngành, c = stt
        bool isNgu; //trong kỳ có rớt môn ko? 
        int credit; //Số tính chỉ
        int DRL; //Điểm rèn luyện

    public:
        Student(string name,
                float gpa_4, //GPA hệ 4
                float gpa_10,//GPA hệ 10
                bool gender, //true = nam, false = nữ
                string dateOfBirth, // string d/month/year sepparator bằng dấu "/"
                string major, //Tên ngành
                string MSSV, //MSSV, "aabbbccc". a = niên khóa, b = ngành, c = stt
                bool isNgu, //trong kỳ có rớt môn ko? 
                int credit, //Số tính chỉ
                int DRL){
            this->name = name;
            this->gpa_4 = gpa_4;
            this->gpa_10 = gpa_10;
            this->gender = gender;
            this->dateOfBirth = dateOfBirth;
            this->major = major;
            this->MSSV = MSSV;
            this->isNgu = isNgu;
            this->credit = credit;
            this->DRL = DRL;
        }
        // ========== GETTER ==========
        string getName() const { return name; }
        float getGPA4() const { return gpa_4; }
        float getGPA10() const { return gpa_10; }
        bool getGender() const { return gender; }
        string getDateOfBirth() const { return dateOfBirth; }
        string getMajor() const { return major; }
        string getMSSV() const { return MSSV; }
        bool getIsNgu() const { return isNgu; }
        int getCredit() const { return credit; }
        int getDRL() const { return DRL; }

        // ========== SETTER (dùng khi phúc khảo) ==========
        void setGPA4(float g) { gpa_4 = g; }
        void setGPA10(float g) { gpa_10 = g; }
        void setDRL(int d) { DRL = d; }
        void setIsNgu(bool v) { isNgu = v; }
        void setCredit(int c) { credit = c; }
};
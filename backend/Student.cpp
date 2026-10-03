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
        string cohort; // Tên khóa 
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
                string cohort, // tên khóa  
                bool isNgu,//trong kỳ có rớt môn ko? 
                int credit, //Số tính chỉ
                int DRL){
            this->name = name;
            this->gpa_4 = gpa_4;
            this->gpa_10 = gpa_10;
            this->gender = gender;
            this->dateOfBirth = dateOfBirth;
            this->major = major;
			this->cohort = cohort;  
            this->MSSV = MSSV;
            this->isNgu = isNgu;
            this->credit = credit;
            this->DRL = DRL;
        }

        //GETTER
        string get_Name() const {
            return name;
        }

        float get_GPA4() const {
            return gpa_4;
        }

        float get_GPA10() const {
            return gpa_10;
        }

        bool get_Gender() const {
            return gender;
        }

        string get_DateOfBirth() const {
            return dateOfBirth;
        }
        
        string get_Cohort() const {
            return cohort;
        }   

        string get_Major() const {
            return major;
        }

        string get_MSSV() const {
            return MSSV;
        }

        bool get_IsNgu() const {
            return isNgu;
        }

        int get_Credit() const {
            return credit;
        }

        int get_DRL() const {
            return DRL;
        }

        //SETTER
        void set_Name(string value) {
            name = value;
        }

        void set_GPA4(float value) {
            gpa_4 = value;
        }

        void set_GPA10(float value) {
            gpa_10 = value;
        }

        void set_Gender(bool value) {
            gender = value;
        }

        void set_DateOfBirth(string value) {
            dateOfBirth = value;
        }

        void set_Major(string value) {
            major = value;
        }
        void set_Cohort(const string& value) {
            cohort = value;
        }

        void set_IsNgu(bool value) {
            isNgu = value;
        }

        void set_Credit(int value) {
            credit = value;
        }

        void set_DRL(int value) {
            DRL = value;
        }

};
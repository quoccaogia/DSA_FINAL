struct StudentComparator {
    // demo cac tieu chi 
    bool operator()(const Student& a, const Student& b) const {

        // 1. Nếu GPA khác nhau -> Đứa nào GPA lớn hơn thì ưu tiên lên trước
        if (a.gpa != b.gpa) {
            return a.gpa > b.gpa;
        }

        // 2. Trái lại, nếu GPA bằng nhau, xét tới Điểm rèn luyện
        if (a.renLuyen != b.renLuyen) {
            return a.renLuyen > b.renLuyen;
        }

        // 3. Cuối cùng, nếu cả GPA và rèn luyện đều bằng nhau -> Xếp theo MSSV tăng dần (để phân biệt)
        return a.mssv < b.mssv;
    }
};
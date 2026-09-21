const USE_MOCK = true;
//mot nhap cai url cua backend vao
const API_URL = "https://jsonplaceholder.typicode.com/posts";

const mockDatabase = {
  "21110001": {
    MSSV: "21110001",
    name: "Nguyễn Văn A",
    major: "Công nghệ thông tin",
    dateOfBirth: "15/08/2003",
    gender: true,
    gpa_4: 3.55,
    gpa_10: 8.85,
    credit: 120,
    DRL: 90
  }
};
//find le button//
const searchButton = document.getElementById('searchButton');
//listen for click event//
searchButton.addEventListener('click', function() {
    const id = document.getElementById('studentId').value;
    console.log(id); 
    //check mssv 8 ky tu va phai la so //
    if(id.length != 8 || !/^\d+$/.test(id)) {
        console.log("MSSV không hợp lệ");
        return;
    }

    const student = mockDatabase[id];
    console.log(student);
    if(!student){
        console.log("Không tìm thấy sinh viên");
        return;
      }

    document.getElementById('result').textContent = student.name;//shit
});

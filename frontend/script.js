const API_URL = "http://localhost:8080/api/student";

const showGPA = (p) => {
  if (p === undefined || p === null) return "-";
  
  return p.toFixed(2);
};


// Chuyển đổi Tab giao diện
function switchTab(tabId, element) {
  document.querySelectorAll('.tab-content').forEach(tab => tab.classList.remove('active'));
  document.querySelectorAll('.tab-btn').forEach(btn => btn.classList.remove('active'));
  
  const targetTab = document.getElementById(tabId);
  if (targetTab) targetTab.classList.add('active');
  if (element) element.classList.add('active');
}

// Lấy giá trị input hoặc chuyển ô trống/không hợp lệ thành null
function getValueOrNull(elementId, type = 'string') {
  const el = document.getElementById(elementId);
  if (!el) return null;

  const val = el.value.trim();
  if (val === "") return null;

  if (type === 'int') {
    const parsed = parseInt(val, 10);
    return isNaN(parsed) ? null : parsed;
  }
  if (type === 'float') {
    const parsed = parseFloat(val);
    return isNaN(parsed) ? null : parsed;
  }
  if (type === 'bool') {
    return val === 'true';
  }
  
  return val;
}

//Top học bổng theo ngành/khóa (hiện sau khi tra cứu)
async function fetchTopKGroup(major, cohort) {
  console.log("fetchTopKGroup CALLED:", major, cohort);

  const card = document.getElementById('topkGroupCard');
  const body = document.getElementById('topkGroupBody');
  const title = document.getElementById('topkGroupTitle');
  if (!card || !body) return;

  card.style.display = 'block';
  if (title) title.textContent = "Top học bổng ngành " + (major ?? "?") + " - khóa " + (cohort ?? "?");

  const showMessage = (msg) => {
    body.innerHTML = "";
    const cell = body.insertRow().insertCell();
    cell.colSpan = 7;
    cell.textContent = msg;
  };

  if (!major || !cohort) {
    showMessage("Chưa có thông tin ngành/khóa của sinh viên.");
    console.log("Dung o day");
    return;
  }

  showMessage("Đang tải...");

  const payload = {
    ACTION: "GET_TOP_K_MAJOR_COHORT",
    major: major,
    cohort: cohort
  };

  let data;
  try {
    const response = await fetch(API_URL, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload)
    });
    if (!response.ok) throw new Error();
    data = await response.json();
  } catch (error) {
    showMessage("Chưa kết nối được Backend.");
    return;
  }

  const list = Array.isArray(data) ? data : (data.students ?? []);

  if (list.length === 0) {
    console.log("dung o list.length == 0");
    showMessage("Chưa có sinh viên nào.");
    return;
  }

  body.innerHTML = "";
  list.forEach((sv, index) => {
    const row = body.insertRow();
    const values = [
      index + 1,
      sv.MSSV ?? sv.mssv,
      sv.name,
      sv.major,
      showGPA(sv.gpa_4),
      showGPA(sv.gpa_10),
      sv.DRL ?? sv.drl
    ];
    values.forEach(v => {
      row.insertCell().textContent = v ?? "-";
    });
  });

  console.log("fetchTopKGroup END");
}

//fetchDeleteStudentData
let deleteMssv = "";
async function fetchDeleteStudentData(){
  const searchEl = document.getElementById('searchDeleteMssv');
  if (!searchEl) return;

  const mssv = searchEl.value.trim();
  deleteMssv = mssv;
  if (!mssv) {
    alert("Vui lòng nhập MSSV để tìm kiếm!");
    return;
  }

  let data;
    // Gọi API thật từ Backend
  try {
    const payload = {
      ACTION: "get_StudentInfo2",
      MSSV: mssv
    }

    const response = await fetch(API_URL, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload)
    });

    if (!response.ok) throw new Error("Không thể lấy dữ liệu.");
    const raw = await response.text();

    console.log("STATUS:", response.status);
    console.log("RAW RESPONSE:", raw);

    if (!raw.trim()) {
        throw new Error("Backend trả response rỗng!");
    }

    data = JSON.parse(raw);
  } 
    catch (error) {
     alert("Lỗi kết nối Backend: " + error.message);
     return;
  
  }

  // Hiển thị bảng kết quả
  const resultCard = document.getElementById('studentDeleteResultCard');
  if (resultCard) resultCard.style.display = 'block';

  // Hàm phụ hỗ trợ ghi text an toàn
  const setField = (id, value) => {
    const el = document.getElementById(id);
    if (el) el.textContent = value;
  };

  setField('del_res_name', data.name ?? "Chưa có (null)");
  setField('del_res_mssv', data.MSSV ?? mssv);
  setField('del_res_major', data.major ?? "Chưa có (null)");
  setField('del_res_dob', data.dateOfBirth ?? "Chưa có (null)");
  setField('del_res_gender', data.gender === true ? "Nam" : (data.gender === false ? "Nữ" : "Chưa có (null)"));
  setField('del_res_gpa4', data.gpa_4 ?? "Chưa có (null)");
  setField('del_res_gpa10', data.gpa_10 ?? "Chưa có (null)");
  setField('del_res_credit', data.credit ?? "Chưa có (null)");
  setField('del_res_cohort', data.cohort ?? "Chưa có (null)");
  setField('del_res_isNgu', data.isNgu === true ? "Có rớt" : (data.isNgu === false ? "Không rớt" : "Chưa có (null)"));
  setField('del_res_drl', data.DRL ?? "Chưa có (null)");

}

// ================================TAB 1: Tra cứu thông tin sinh viên
async function fetchStudentData() {
  console.log("fetchStudentData CALLED");
  const searchEl = document.getElementById('searchMssv');
  if (!searchEl) return;

  const mssv = searchEl.value.trim();

  if (!mssv) {
    alert("Vui lòng nhập MSSV để tìm kiếm!");
    return;
  }

  let data;
    // Gọi API thật từ Backend
  try {
    const payload = {
      ACTION: "get_StudentInfo",
      MSSV: mssv
    }

    const response = await fetch(API_URL, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload)
    });

    if (!response.ok) throw new Error("Không thể lấy dữ liệu.");
    data = await response.json();
  } 
    catch (error) {
     alert("Lỗi kết nối Backend: " + error.message);
     return;
  }

  // Hiển thị bảng kết quả
  const resultCard = document.getElementById('studentResultCard');
  if (resultCard) resultCard.style.display = 'block';

  // Hàm phụ hỗ trợ ghi text an toàn
  const setField = (id, value) => {
    const el = document.getElementById(id);
    if (el) el.textContent = value;
  };

  setField('res_name', data.name ?? "Chưa có (null)");
  setField('res_mssv', data.MSSV ?? mssv);
  setField('res_major', data.major ?? "Chưa có (null)");
  setField('res_dob', data.dateOfBirth ?? "Chưa có (null)");
  setField('res_gender', data.gender === true ? "Nam" : (data.gender === false ? "Nữ" : "Chưa có (null)"));
  setField('res_gpa4', showGPA(data.gpa_4) ?? "Chưa có (null)");
  setField('res_gpa10', showGPA(data.gpa_10) ?? "Chưa có (null)");
  setField('res_credit', data.credit ?? "Chưa có (null)");
  setField('res_drl', data.DRL ?? "Chưa có (null)");

  const showPercent = (p) => {
    if (p === undefined || p === null) return "Chưa có (null)";
    if (p < 0) return "Không đủ điều kiện";
    return "Top " + p.toFixed(1) + "%";
  };


  setField('res_topAll', showPercent(data.topPercentAllSchool));

  setField('res_topGroup', showPercent(data.topPercentGroup));

  fetchTopKGroup(data.major, data.cohort);

  console.log("fetchStudentData END");
}

// ===========================TAB 2: Gửi cập nhật thông tin sinh viên
async function sendStudentUpdate() {
  const payload = {
    ACTION: "UPDATE_STUDENT",
    MSSV: getValueOrNull('up_mssv', 'string'),
    name: getValueOrNull('up_name', 'string'),
    major: getValueOrNull('up_major', 'string'),
    dateOfBirth: getValueOrNull('up_dob', 'string'),
    gender: getValueOrNull('up_gender', 'bool'),
    credit: getValueOrNull('up_credit', 'int'),
    gpa_4: getValueOrNull('up_gpa4', 'float'),
    gpa_10: getValueOrNull('up_gpa10', 'float'),
    DRL: getValueOrNull('up_drl', 'int'),
    cohort: getValueOrNull('up_cohort', 'string'),
    isNgu: getValueOrNull('up_isNgu', 'bool'),
  };

  try {
    const response = await fetch(API_URL, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload)
    });
    if (!response.ok) throw new Error("Cập nhật thất bại.");
    alert("Cập nhật thành công!");
  } 
  catch (error) {
    alert("Lỗi khi gửi dữ liệu: " + error.message);
  }

  //Refesh bảng top K
  fetchTopK();
}

async function deleteStudent() {
    const mssv = deleteMssv;

    if (!mssv) {
        alert("Vui lòng nhập MSSV để xóa!");
        return;
    }

    if (!confirm("Bạn có chắc muốn xóa sinh viên " + mssv + "?")) return;

    const payload = {
        ACTION: "DELETE_STUDENT",
        MSSV: mssv
    };

    try {
        const response = await fetch(API_URL, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(payload)
        });
        if (!response.ok) throw new Error("Không tìm thấy sinh viên hoặc xóa thất bại.");
        alert("Đã xóa sinh viên " + mssv + "!");
    } catch (error) {
        alert("Lỗi khi xóa: " + error.message);
        return;
    }

    // SỬA MỤC 4: Đổi 'del_mssv' thành 'searchDeleteMssv' đúng theo ID trong HTML
    const searchInput = document.getElementById('searchDeleteMssv');
    if (searchInput) searchInput.value = "";

    const delCard = document.getElementById('studentDeleteResultCard');
    if (delCard) delCard.style.display = 'none';

    deleteMssv = "";
    fetchTopK();
}

// Nhập CSV
async function importCSV() {
  console.log("importCSV CALLED");
  const csvFile = document.getElementById('csvFile');
  if (!csvFile) return;
  const file = csvFile.files[0];

  if (!file) {
    alert("Vui lòng chọn file CSV.");
    return;
  }

  const text = await file.text();
  const lines = text.split("\n");
  const headers = lines[0].trim().split(",");

  // Kiểu dữ liệu của từng cột (giống sendStudentUpdate)
    const types = {
    credit: 'int', drl: 'int',
    gpa_4: 'float', gpa_10: 'float',
    gender: 'bool', has_failed: 'bool'
  };

  const students = [];

  for (let i = 1; i < lines.length; i++) {
    const line = lines[i].trim();
    if (line === "") continue;

    const values = line.split(",");
    const student = {};

    for (let j = 0; j < headers.length; j++) {
      const key = headers[j].trim();
      const raw = (values[j] ?? "").trim();
            const type = types[key];

      if (raw === "") {
        student[key] = null;
      } else if (type === 'int') {
        const n = parseInt(raw, 10);
        student[key] = isNaN(n) ? null : n;
      } else if (type === 'float') {
        const n = parseFloat(raw);
        student[key] = isNaN(n) ? null : n;
      } else if (type === 'bool') {
        student[key] = raw === 'true';
      } else {
        student[key] = raw;
      }
    }

    students.push(student);
  }

  const payload = {
    ACTION: "UPLOAD BASE DATA",
    CSV: students
  };

  try {
    const response = await fetch(API_URL, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload)
    });
    if (!response.ok) throw new Error("Upload thất bại.");
    alert("Đã gửi " + students.length + " sinh viên!");
  }
  catch (error) {
    alert("Lỗi khi gửi dữ liệu: " + error.message);
  }

  console.log("importCSV END");
 }

// ===========================Top học bổng (tự tải khi mở trang)
async function fetchTopK() {
  const body = document.getElementById('topkBody');
  if (!body) return;

  const showMessage = (msg) => {
    body.innerHTML = "";
    const cell = body.insertRow().insertCell();
    cell.colSpan = 7;
    cell.textContent = msg;
  };

  const payload = {
    ACTION: "GET_TOP_K"
  };

  let data;

  try {
    const response = await fetch(API_URL, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload)
    });
    if (!response.ok) throw new Error();
    data = await response.json();
  } 
  catch (error) {
    showMessage("Chưa kết nối được Backend.");
    return;
  }

  const list = Array.isArray(data) ? data : (data.students ?? []);
  if (list.length === 0) {
    showMessage("Chưa có sinh viên nào.");
    return;
  }

  body.innerHTML = "";
  list.forEach((sv, index) => {
    const row = body.insertRow();
    const values = [
      index + 1,
      sv.MSSV ?? sv.mssv,
      sv.name,
      sv.major,
      showGPA(sv.gpa_4),
      showGPA(sv.gpa_10),
      sv.DRL ?? sv.drl
    ];
    values.forEach(v => {
      row.insertCell().textContent = v ?? "-";
    });
  });
}

// ============= add student
async function addStudent(){
  const requiredFields = [
        'add_mssv',
        'add_name',
        'add_major',
        'add_dob',
        'add_gender',
        'add_credit',
        'add_gpa4',
        'add_gpa10',
        'add_drl',
        'add_cohort',
        'add_isNgu'
    ];

  for (const id of requiredFields) {
      const el = document.getElementById(id);

      if (!el || el.value.trim() === "") {
          alert("Vui lòng nhập đầy đủ thông tin!");
          el?.focus();
          return;
      }
  }

  const payload = {
    ACTION: "ADD_STUDENT",
    MSSV: document.getElementById('add_mssv').value.trim(),
    name: document.getElementById('add_name').value.trim(),
    major: document.getElementById('add_major').value.trim(),
    dateOfBirth: document.getElementById('add_dob').value.trim(),
    gender: document.getElementById('add_gender').value === "true",
    credit: Number(document.getElementById('add_credit').value),
    gpa_4: Number(document.getElementById('add_gpa4').value),
    gpa_10: Number(document.getElementById('add_gpa10').value),
    DRL: Number(document.getElementById('add_drl').value),
    cohort: document.getElementById('add_cohort').value.trim(),
    isNgu: document.getElementById('add_isNgu').value === "true"
  };

  try {
    console.log(JSON.stringify(payload, null, 2));
    const response = await fetch(API_URL, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload)
    });
    if (!response.ok) throw new Error("Cập nhật thất bại.");
    alert("Cập nhật thành công!");
  } 
  catch (error) {
    alert("Lỗi khi gửi dữ liệu: " + error.message);
  }

  //Refesh bảng topK
  fetchTopK();
}
// Tự chạy khi mở trang
fetchTopK();

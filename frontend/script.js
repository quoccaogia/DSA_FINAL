const API_URL = "http://localhost:8080/api/student";

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

// ================================TAB 1: Tra cứu thông tin sinh viên
async function fetchStudentData() {
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
  setField('res_gpa4', data.gpa_4 ?? "Chưa có (null)");
  setField('res_gpa10', data.gpa_10 ?? "Chưa có (null)");
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
    DRL: getValueOrNull('up_drl', 'int')
  };

  try {
      const response = await fetch(API_URL, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(payload)
      });
      if (!response.ok) throw new Error("Cập nhật thất bại.");
      alert("Cập nhật thành công!");
    } catch (error) {
      alert("Lỗi khi gửi dữ liệu: " + error.message);
    }
  }

// TAB 2: Nhập CSV
async function importCSV() {
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

  console.log(students); // kiểm tra trước khi gửi

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
  } catch (error) {
    alert("Lỗi khi gửi dữ liệu: " + error.message);
  }


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
  } catch (error) {
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
      sv.gpa_4,
      sv.gpa_10,
      sv.DRL ?? sv.drl
    ];
    values.forEach(v => {
      row.insertCell().textContent = v ?? "-";
    });
  });
}

// Tự chạy khi mở trang
fetchTopK();

// ===========================Top học bổng theo ngành/khóa (hiện sau khi tra cứu)
async function fetchTopKGroup(major, cohort) {
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
    return;
  }

  showMessage("Đang tải...");

  const payload = {
    ACTION: "GET_TOP_K",
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
      sv.gpa_4,
      sv.gpa_10,
      sv.DRL ?? sv.drl
    ];
    values.forEach(v => {
      row.insertCell().textContent = v ?? "-";
    });
  });
}
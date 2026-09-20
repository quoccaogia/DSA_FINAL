const USE_MOCK = true;

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

// TAB 1: Tra cứu thông tin sinh viên
async function fetchStudentData() {
  const searchEl = document.getElementById('searchMssv');
  if (!searchEl) return;

  const mssv = searchEl.value.trim();

  if (!mssv) {
    alert("Vui lòng nhập MSSV để tìm kiếm!");
    return;
  }

  let data;

  if (USE_MOCK) {
    // Giả lập lấy dữ liệu từ memory
    data = mockDatabase[mssv] || {
      MSSV: mssv,
      name: null,
      major: null,
      dateOfBirth: null,
      gender: null,
      gpa_4: null,
      gpa_10: null,
      credit: null,
      DRL: null
    };
  } else {
    // Gọi API thật từ Backend
    try {
      const response = await fetch(API_URL, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ MSSV: mssv })
      });
      if (!response.ok) throw new Error("Không thể lấy dữ liệu.");
      data = await response.json();
    } catch (error) {
      alert("Lỗi kết nối Backend: " + error.message);
      return;
    }
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
}

// TAB 2: Gửi cập nhật thông tin sinh viên
async function sendStudentUpdate() {
  const payload = {
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

  if (USE_MOCK) {
    alert("[MOCK SUCCESS]\nCập nhật dữ liệu thành công!\n\nPayload JSON đã tạo:\n" + JSON.stringify(payload, null, 2));
    
    // Lưu tạm vào bộ nhớ giả lập để test ngay ở Tab 1
    if (payload.MSSV) {
      mockDatabase[payload.MSSV] = payload;
    }
  } else {
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
}
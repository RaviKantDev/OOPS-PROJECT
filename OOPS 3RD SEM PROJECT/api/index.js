/**
 * CampusFlow - Vercel Cloud Serverless API Bridge
 * Mirrors the native C++ Backend Engine on Vercel
 * B.Tech IT-B (3rd Semester) Edition
 */

// In-Memory Cloud State
let users = [
  {
    id: "u_cr01",
    username: "priyanshu_cr",
    password: "priyanshu123",
    fullName: "Priyanshu Raj (CR)",
    rollNumber: "23ITB01",
    enrollmentNumber: "0101IT231001",
    semester: 3,
    branch: "Information Technology",
    section: "IT-B",
    role: "CR",
    canManageAssignments: true,
    canDeclareHolidays: true
  },
  {
    id: "u_st01",
    username: "prakash",
    password: "prakash123",
    fullName: "Prakash Kumar",
    rollNumber: "23ITB25",
    enrollmentNumber: "0101IT231025",
    semester: 3,
    branch: "Information Technology",
    section: "IT-B",
    role: "STUDENT",
    canManageAssignments: false,
    canDeclareHolidays: false
  },
  {
    id: "u_st02",
    username: "ravikant",
    password: "ravi123",
    fullName: "Ravikant",
    rollNumber: "23ITB38",
    enrollmentNumber: "0101IT231038",
    semester: 3,
    branch: "Information Technology",
    section: "IT-B",
    role: "STUDENT",
    canManageAssignments: false,
    canDeclareHolidays: false
  }
];

let assignments = [
  {
    id: "asgn_101",
    subjectCode: "IT301",
    subjectName: "Object Oriented Programming (C++)",
    title: "Polymorphism, Virtual Functions & Exception Handling",
    description: "Implement base class User with dynamic dispatch methods. Write complete report with class hierarchy diagrams.",
    assignedDate: "2026-10-20",
    deadlineDate: "2026-10-29",
    submissionMedium: "Practical Copy",
    facultyName: "Dr. Rajesh Kumar",
    maxMarks: 25,
    instructions: "Handwritten on practical lab copy with blue/black pen only."
  },
  {
    id: "asgn_102",
    subjectCode: "IT302",
    subjectName: "Data Structures & Algorithms",
    title: "AVL Tree Balancing & Dijkstra Shortest Path Trace",
    description: "Solve the 5 problems on balancing factors and write step-by-step trace of Dijkstra's algorithm for given graph.",
    assignedDate: "2026-10-22",
    deadlineDate: "2026-10-31",
    submissionMedium: "Stick File",
    facultyName: "Prof. Anita Sen",
    maxMarks: 20,
    instructions: "Punch A4 ruled sheets inside a standard stick file. Write Roll No on top-right."
  },
  {
    id: "asgn_103",
    subjectCode: "IT303",
    subjectName: "Computer Organization & Architecture",
    title: "Booth Multiplication & Cache Mapping Problems",
    description: "Solve numericals on 2-way set associative cache hit/miss and trace 8-bit Booth multiplication algorithm.",
    assignedDate: "2026-10-18",
    deadlineDate: "2026-10-28",
    submissionMedium: "Assignment Copy",
    facultyName: "Dr. S. K. Mitra",
    maxMarks: 20,
    instructions: "Write in IT-B dedicated assignment register."
  },
  {
    id: "asgn_104",
    subjectCode: "IT304",
    subjectName: "Discrete Mathematics & Graph Theory",
    title: "Recurrence Relations & Planar Graphs Tutorial #4",
    description: "Solve problems 1 to 12 from tutorial sheet 4 on second order homogeneous recurrence relations.",
    assignedDate: "2026-10-21",
    deadlineDate: "2026-11-03",
    submissionMedium: "Loose Graph Sheets",
    facultyName: "Prof. Manoj Tripathi",
    maxMarks: 20,
    instructions: "Submit 2 graph sheets clamped together with header slip."
  },
  {
    id: "asgn_105",
    subjectCode: "IT305",
    subjectName: "Digital Logic & Design",
    title: "Mod-10 Synchronous Counter Design & State Diagram",
    description: "Design synchronous Mod-10 counter using JK Flip-Flops. Draw complete circuit diagram and truth table.",
    assignedDate: "2026-10-23",
    deadlineDate: "2026-11-06",
    submissionMedium: "Spiral File",
    facultyName: "Dr. Neha Kapoor",
    maxMarks: 20,
    instructions: "Spiral bound with printed cover page and signed acknowledgment."
  }
];

let holidays = [
  { date: "2026-10-14", description: "Diwali Pre-Break & Cultural Fest", declaredBy: "Priyanshu Raj (CR)" },
  { date: "2026-10-30", description: "Annual IT Department Sports Meet", declaredBy: "Priyanshu Raj (CR)" },
  { date: "2026-11-04", description: "University Foundation Day", declaredBy: "Priyanshu Raj (CR)" }
];

const defaultSubjects = [
  "Object Oriented Programming (C++)",
  "Data Structures & Algorithms",
  "Computer Organization & Architecture",
  "Discrete Mathematics & Graph Theory",
  "Digital Logic & Design"
];

let studentCustomSubjects = {
  "u_st01": ["Data Structures Lab (C++)"],
  "u_st02": ["Python Programming Elective"]
};

let attendanceRecords = [];

// Populate default records
(function initDefaultAttendance() {
  const dates = ["2026-10-19", "2026-10-20", "2026-10-21", "2026-10-22", "2026-10-23"];
  const studs = ["u_st01", "u_st02", "u_cr01"];
  let count = 1;
  studs.forEach((stId, sIdx) => {
    dates.forEach((d, dIdx) => {
      defaultSubjects.forEach((sub, subIdx) => {
        let st = "PRESENT";
        if (sIdx === 0 && subIdx === 2 && dIdx === 2) st = "ABSENT";
        if (sIdx === 0 && subIdx === 3 && dIdx === 4) st = "ABSENT";
        if (sIdx === 1 && subIdx === 1 && dIdx === 1) st = "ABSENT";
        if (sIdx === 1 && subIdx === 4 && dIdx === 3) st = "CANCELLED";
        attendanceRecords.push({
          id: `att_${count++}`,
          studentId: stId,
          subjectName: sub,
          date: d,
          status: st,
          timestamp: "09:00 AM"
        });
      });
    });
  });
})();

// Date Utilities
function getTodayDate() {
  const now = new Date();
  return now.toISOString().split("T")[0];
}

function getDayName(dateStr) {
  const d = new Date(dateStr + "T00:00:00");
  return d.toLocaleDateString("en-US", { weekday: "long" });
}

function isWeekend(dateStr) {
  const d = new Date(dateStr + "T00:00:00");
  const day = d.getDay();
  return day === 0 || day === 6;
}

// Handler
module.exports = async (req, res) => {
  // CORS
  res.setHeader("Access-Control-Allow-Origin", "*");
  res.setHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS, DELETE");
  res.setHeader("Access-Control-Allow-Headers", "Content-Type");

  if (req.method === "OPTIONS") {
    return res.status(200).end();
  }

  const url = new URL(req.url, `http://${req.headers.host}`);
  const pathname = url.pathname;

  let body = req.body || {};
  if (typeof body === "string") {
    try { body = JSON.parse(body); } catch(e) {}
  }

  // 1. Meta
  if (pathname === "/api/meta" && req.method === "GET") {
    const today = getTodayDate();
    return res.status(200).json({
      todayDate: today,
      todayDay: getDayName(today),
      isWeekend: isWeekend(today),
      course: "B.Tech",
      branch: "Information Technology",
      section: "IT-B",
      semester: 3,
      crName: "Priyanshu Raj",
      subjects: defaultSubjects
    });
  }

  // 2. Login
  if (pathname === "/api/login" && req.method === "POST") {
    const { username, password } = body;
    const user = users.find(u => u.username === username && u.password === password);
    if (user) {
      return res.status(200).json({ success: true, user });
    } else {
      return res.status(400).json({ success: false, error: "Invalid username or password" });
    }
  }

  // 3. Register
  if (pathname === "/api/register" && req.method === "POST") {
    let { username, password, enrollmentNumber, fullName, rollNumber, semester, branch, section } = body;
    if (!username || !password || !enrollmentNumber) {
      return res.status(400).json({ success: false, error: "Username, password and enrollment number are required" });
    }
    if (users.find(u => u.username === username)) {
      return res.status(400).json({ success: false, error: "Username already exists" });
    }
    if (!fullName) fullName = username;
    if (!rollNumber) rollNumber = enrollmentNumber.length >= 2 ? `23ITB${enrollmentNumber.slice(-2)}` : "23ITB00";

    const newUser = {
      id: `u_st${users.length + 1}`,
      username,
      password,
      fullName,
      rollNumber,
      enrollmentNumber,
      semester: semester || 3,
      branch: branch || "Information Technology",
      section: section || "IT-B",
      role: "STUDENT",
      canManageAssignments: false,
      canDeclareHolidays: false
    };
    users.push(newUser);
    return res.status(200).json({ success: true, message: "Account registered successfully", user: newUser });
  }

  // 4. Profile Update
  if (pathname === "/api/profile/update" && req.method === "POST") {
    const { userId, fullName, rollNumber, enrollmentNumber, semester, branch, section } = body;
    const u = users.find(x => x.id === userId);
    if (!u) return res.status(404).json({ success: false, error: "User not found" });

    if (fullName) u.fullName = fullName;
    if (rollNumber) u.rollNumber = rollNumber;
    if (enrollmentNumber) u.enrollmentNumber = enrollmentNumber;
    if (semester) u.semester = semester;
    if (branch) u.branch = branch;
    if (section) u.section = section;

    return res.status(200).json({ success: true, message: "Profile updated successfully", user: u });
  }

  // 5. Subjects
  if (pathname === "/api/subjects" && req.method === "GET") {
    const studentId = url.searchParams.get("studentId") || "u_st01";
    const extra = studentCustomSubjects[studentId] || [];
    const combined = [...defaultSubjects];
    extra.forEach(s => {
      if (!combined.includes(s)) combined.push(s);
    });
    return res.status(200).json(combined);
  }

  // 6. Add Custom Subject
  if (pathname === "/api/subjects/add" && req.method === "POST") {
    const { studentId, subjectName } = body;
    if (!studentId || !subjectName) {
      return res.status(400).json({ success: false, error: "Student ID and Subject Name are required" });
    }
    if (!studentCustomSubjects[studentId]) {
      studentCustomSubjects[studentId] = [];
    }
    const cur = [...defaultSubjects, ...(studentCustomSubjects[studentId])];
    if (cur.includes(subjectName.trim())) {
      return res.status(400).json({ success: false, error: "Subject already enrolled" });
    }
    studentCustomSubjects[studentId].push(subjectName.trim());
    return res.status(200).json({ success: true, message: "Subject added successfully to your curriculum" });
  }

  // 7. Assignments
  if (pathname === "/api/assignments" && req.method === "GET") {
    const sorted = [...assignments].sort((a, b) => a.deadlineDate.localeCompare(b.deadlineDate));
    return res.status(200).json(sorted);
  }

  if (pathname === "/api/assignments" && req.method === "POST") {
    const { subjectCode, subjectName, title, description, assignedDate, deadlineDate, submissionMedium, facultyName, maxMarks, instructions } = body;
    if (!subjectName || !title || !deadlineDate || !submissionMedium) {
      return res.status(400).json({ success: false, error: "Required fields missing" });
    }
    const newA = {
      id: `asgn_${Date.now() % 100000}`,
      subjectCode: subjectCode || "IT301",
      subjectName,
      title,
      description: description || "",
      assignedDate: assignedDate || getTodayDate(),
      deadlineDate,
      submissionMedium,
      facultyName: facultyName || "Faculty",
      maxMarks: maxMarks || 20,
      instructions: instructions || "Submit on time"
    };
    assignments.push(newA);
    return res.status(200).json({ success: true, message: "Assignment created successfully" });
  }

  if (pathname === "/api/assignments/delete" && req.method === "POST") {
    const { id } = body;
    const idx = assignments.findIndex(a => a.id === id);
    if (idx !== -1) {
      assignments.splice(idx, 1);
      return res.status(200).json({ success: true, message: "Assignment deleted successfully" });
    }
    return res.status(404).json({ success: false, error: "Assignment not found" });
  }

  // 8. Holidays
  if (pathname === "/api/holidays" && req.method === "GET") {
    return res.status(200).json(holidays);
  }

  if (pathname === "/api/holidays" && req.method === "POST") {
    const { date, description, declaredBy } = body;
    if (!date || !description) return res.status(400).json({ success: false, error: "Date and description required" });
    if (holidays.find(h => h.date === date)) return res.status(400).json({ success: false, error: "Holiday already exists" });

    holidays.push({ date, description, declaredBy: declaredBy || "Priyanshu Raj (CR)" });
    return res.status(200).json({ success: true, message: "Holiday declared successfully" });
  }

  if (pathname === "/api/holidays/delete" && req.method === "POST") {
    const { date } = body;
    const idx = holidays.findIndex(h => h.date === date);
    if (idx !== -1) {
      holidays.splice(idx, 1);
      return res.status(200).json({ success: true, message: "Holiday removed successfully" });
    }
    return res.status(404).json({ success: false, error: "Holiday not found" });
  }

  // 9. Attendance
  if (pathname === "/api/attendance" && req.method === "GET") {
    const studentId = url.searchParams.get("studentId") || "u_st01";
    const extra = studentCustomSubjects[studentId] || [];
    const subjects = [...defaultSubjects];
    extra.forEach(s => { if (!subjects.includes(s)) subjects.push(s); });

    const studentRecords = attendanceRecords.filter(r => r.studentId === studentId);

    let totalAttended = 0;
    let totalHeld = 0;

    const subjectStats = subjects.map(sub => {
      let attended = 0;
      let absent = 0;
      let cancelled = 0;

      studentRecords.forEach(r => {
        if (r.subjectName === sub) {
          if (r.status === "PRESENT") attended++;
          else if (r.status === "ABSENT") absent++;
          else if (r.status === "CANCELLED") cancelled++;
        }
      });

      const held = attended + absent;
      totalAttended += attended;
      totalHeld += held;

      const pct = held > 0 ? (attended * 100) / held : 100.0;
      let safeMisses75 = 0, req75 = 0;
      let safeMisses65 = 0, req65 = 0;

      if (pct >= 75) {
        safeMisses75 = Math.floor((100 * attended - 75 * held) / 75);
      } else {
        req75 = Math.ceil((75 * held - 100 * attended) / 25);
      }

      if (pct >= 65) {
        safeMisses65 = Math.floor((100 * attended - 65 * held) / 65);
      } else {
        req65 = Math.ceil((65 * held - 100 * attended) / 35);
      }

      return {
        subjectName: sub,
        attendedCount: attended,
        absentCount: absent,
        cancelledCount: cancelled,
        totalHeld: held,
        percentage: parseFloat(pct.toFixed(1)),
        safeMisses75,
        requiredAttend75: req75,
        safeMisses65,
        requiredAttend65: req65,
        statusTag: pct >= 75 ? "Safe" : pct >= 65 ? "Warning" : "Critical"
      };
    });

    const overallPercentage = totalHeld > 0 ? parseFloat(((totalAttended * 100) / totalHeld).toFixed(1)) : 100.0;

    return res.status(200).json({
      overallPercentage,
      totalAttended,
      totalHeld,
      subjectStats,
      history: studentRecords
    });
  }

  // 10. Mark Attendance
  if (pathname === "/api/attendance" && req.method === "POST") {
    const { studentId, subjectName, date, status } = body;
    const today = getTodayDate();

    // Check future date
    if (date > today) {
      return res.status(400).json({ success: false, error: `Cannot mark attendance on future date: ${date}` });
    }

    // Check weekend
    if (isWeekend(date)) {
      return res.status(400).json({ success: false, error: `${date} is a Weekend. Saturday & Sunday are non-instructional days.` });
    }

    // Check CR holiday
    const matchH = holidays.find(h => h.date === date);
    if (matchH) {
      return res.status(400).json({ success: false, error: `${date} is an official holiday: ${matchH.description}` });
    }

    // Record or update
    const existing = attendanceRecords.find(r => r.studentId === studentId && r.subjectName === subjectName && r.date === date);
    if (existing) {
      existing.status = status;
    } else {
      attendanceRecords.push({
        id: `att_${Date.now()}`,
        studentId,
        subjectName,
        date,
        status,
        timestamp: "Logged"
      });
    }

    return res.status(200).json({ success: true, message: "Attendance marked successfully" });
  }

  return res.status(404).json({ error: "Endpoint not found" });
};


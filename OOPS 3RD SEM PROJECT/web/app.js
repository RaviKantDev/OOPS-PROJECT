/**
 * CampusFlow - Frontend Application Logic
 * B.Tech IT-B (3rd Semester) Edition
 * Connects with Native C++ Winsock REST Backend
 *
 * Member 1: Authentication, Session Privacy & Profile Editing
 * Member 2: Real-time Assignment Hub & Submission Mediums
 * Member 3: Daily Attendance, Dynamic Custom Subjects & Safe Miss Calculations
 */

// Application State
const state = {
  currentUser: null, // null when logged out, populated on login
  meta: {
    todayDate: "",
    todayDay: "",
    course: "B.Tech",
    branch: "Information Technology",
    section: "IT-B",
    semester: 3,
    subjects: []
  },
  studentSubjects: [],
  assignments: [],
  holidays: [],
  attendance: {
    overallPercentage: 100,
    totalAttended: 0,
    totalHeld: 0,
    subjectStats: [],
    history: []
  },
  selectedDate: "",
  doneAssignments: new Set(JSON.parse(localStorage.getItem("campusflow_done") || "[]"))
};

// DOM References
const el = {
  // Auth Screen
  authScreen: document.getElementById("auth-screen"),
  appShell: document.getElementById("app-shell"),
  tabBtnSignin: document.getElementById("tab-btn-signin"),
  tabBtnRegister: document.getElementById("tab-btn-register"),
  formLogin: document.getElementById("form-login"),
  loginUsername: document.getElementById("login-username"),
  loginPassword: document.getElementById("login-password"),
  formRegisterAuth: document.getElementById("form-register-auth"),

  // Top Nav
  todayDatePill: document.getElementById("nav-today-date"),
  currentUserName: document.getElementById("current-user-name"),
  currentUserRole: document.getElementById("current-user-role"),
  currentUserRoll: document.getElementById("current-user-roll"),
  btnOpenEditProfile: document.getElementById("btn-open-edit-profile"),
  btnOpenRegisterModal: document.getElementById("btn-open-register-modal"),
  btnLogout: document.getElementById("btn-logout"),

  // Dashboards
  classmateDashboard: document.getElementById("classmate-dashboard"),
  crDashboard: document.getElementById("cr-dashboard"),
  studentWelcomeName: document.getElementById("student-welcome-name"),
  studentRollNo: document.getElementById("student-roll-no"),
  studentEnrNo: document.getElementById("student-enr-no"),
  studentBranchSec: document.getElementById("student-branch-sec"),
  studentSemVal: document.getElementById("student-sem-val"),
  crWelcomeName: document.getElementById("cr-welcome-name"),

  // Attendance Metrics
  overallPercentageVal: document.getElementById("overall-percentage-val"),
  overallStatusPill: document.getElementById("overall-status-pill"),

  // Assignments
  assignmentSearch: document.getElementById("assignment-search"),
  filterSubject: document.getElementById("filter-subject"),
  filterMedium: document.getElementById("filter-medium"),
  assignmentsContainer: document.getElementById("assignments-container"),

  // Daily Marking & Analytics (Student)
  attendanceDateInput: document.getElementById("attendance-date-input"),
  dateStatusNotice: document.getElementById("date-status-notice"),
  dailySubjectMarkingList: document.getElementById("daily-subject-marking-list"),
  subjectsAnalyticsGrid: document.getElementById("subjects-analytics-grid"),
  btnOpenAddSubject: document.getElementById("btn-open-add-subject"),

  // CR Personal Attendance
  btnOpenAddSubjectCr: document.getElementById("btn-open-add-subject-cr"),
  crAttendanceDateInput: document.getElementById("cr-attendance-date-input"),
  crDateStatusNotice: document.getElementById("cr-date-status-notice"),
  crDailySubjectMarkingList: document.getElementById("cr-daily-subject-marking-list"),
  crSubjectsAnalyticsGrid: document.getElementById("cr-subjects-analytics-grid"),

  // CR Management
  crAssignmentsTbody: document.getElementById("cr-assignments-tbody"),
  holidaysListContainer: document.getElementById("holidays-list-container"),
  btnOpenAddAssignment: document.getElementById("btn-open-add-assignment"),
  modalAddAssignment: document.getElementById("modal-add-assignment"),
  btnCloseAsgnModal: document.getElementById("btn-close-asgn-modal"),
  btnCancelAsgn: document.getElementById("btn-cancel-asgn"),
  formAddAssignment: document.getElementById("form-add-assignment"),
  asgnAssignedDate: document.getElementById("asgn-assigned-date"),

  btnOpenAddHoliday: document.getElementById("btn-open-add-holiday"),
  formAddHoliday: document.getElementById("form-add-holiday"),
  holidayDateInput: document.getElementById("holiday-date"),

  // Modals
  modalEditProfile: document.getElementById("modal-edit-profile"),
  btnCloseProfileModal: document.getElementById("btn-close-profile-modal"),
  btnCancelProfile: document.getElementById("btn-cancel-profile"),
  formEditProfile: document.getElementById("form-edit-profile"),

  modalRegisterStudent: document.getElementById("modal-register-student"),
  btnCloseRegisterModal: document.getElementById("btn-close-register-modal"),
  btnCancelRegModal: document.getElementById("btn-cancel-reg-modal"),
  formRegisterModal: document.getElementById("form-register-modal"),

  modalAddSubject: document.getElementById("modal-add-subject"),
  btnCloseSubjectModal: document.getElementById("btn-close-subject-modal"),
  btnCancelSubject: document.getElementById("btn-cancel-subject"),
  formAddSubject: document.getElementById("form-add-subject"),

  toastContainer: document.getElementById("toast-container")
};

// ============================================================================
// Initialization & Authentication Gate
// ============================================================================
async function init() {
  setupEventListeners();
  await loadMeta();

  // Check saved session in sessionStorage
  const savedUser = sessionStorage.getItem("campusflow_user");
  if (savedUser) {
    try {
      state.currentUser = JSON.parse(savedUser);
      onLoginSuccess(false);
    } catch (e) {
      sessionStorage.removeItem("campusflow_user");
      showAuthScreen();
    }
  } else {
    showAuthScreen();
  }

  // Real-time synchronization interval (every 4.5 seconds)
  setInterval(() => {
    if (state.currentUser) {
      syncRealtimeData();
    }
  }, 4500);
}

function showAuthScreen() {
  el.authScreen.classList.remove("hidden");
  el.appShell.classList.add("hidden");
}

function onLoginSuccess(showWelcomeToast = true) {
  el.authScreen.classList.add("hidden");
  el.appShell.classList.remove("hidden");

  updateNavProfileUI();

  if (state.currentUser.role === "CR") {
    el.classmateDashboard.classList.remove("active");
    el.crDashboard.classList.add("active");
    el.crWelcomeName.textContent = state.currentUser.fullName;
  } else {
    el.crDashboard.classList.remove("active");
    el.classmateDashboard.classList.add("active");
    updateStudentWelcomeBanner();
  }

  if (showWelcomeToast) {
    showToast(`Welcome, ${state.currentUser.fullName}! Logged into IT-B Portal.`, "success");
  }

  refreshAllUserData();
}

function updateNavProfileUI() {
  if (!state.currentUser) return;
  el.currentUserName.textContent = state.currentUser.fullName;
  el.currentUserRole.textContent = state.currentUser.role === "CR" ? "Class Representative" : "Classmate";
  el.currentUserRoll.textContent = state.currentUser.rollNumber || "IT-B";
}

function updateStudentWelcomeBanner() {
  if (!state.currentUser) return;
  el.studentWelcomeName.textContent = state.currentUser.fullName;
  el.studentRollNo.textContent = state.currentUser.rollNumber || "23ITB25";
  el.studentEnrNo.textContent = state.currentUser.enrollmentNumber || "0101IT231025";
  el.studentBranchSec.textContent = `${state.currentUser.branch || "Information Technology"} (${state.currentUser.section || "IT-B"})`;
  el.studentSemVal.textContent = state.currentUser.semester || "3";
}

// 1-Click Quick Fill for testing
window.quickFill = function(username, password) {
  el.loginUsername.value = username;
  el.loginPassword.value = password;
  handleLoginSubmit(new Event("submit"));
};

async function handleLoginSubmit(e) {
  if (e && e.preventDefault) e.preventDefault();
  const username = el.loginUsername.value.trim();
  const password = el.loginPassword.value.trim();

  if (!username || !password) {
    showToast("Please enter both username and password", "error");
    return;
  }

  try {
    const res = await fetch("/api/login", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ username, password })
    });
    const data = await res.json();
    if (!res.ok) throw new Error(data.error || "Authentication failed");

    state.currentUser = data.user;
    sessionStorage.setItem("campusflow_user", JSON.stringify(data.user));
    onLoginSuccess(true);
  } catch (err) {
    showToast(err.message, "error");
  }
}

async function handleRegisterAuthSubmit(e) {
  e.preventDefault();
  const username = document.getElementById("reg-auth-username").value.trim();
  const password = document.getElementById("reg-auth-password").value.trim();
  const enrollmentNumber = document.getElementById("reg-auth-enr").value.trim();
  const fullName = document.getElementById("reg-auth-name").value.trim() || username;
  const rollNumber = document.getElementById("reg-auth-roll").value.trim();

  if (!username || !password || !enrollmentNumber) {
    showToast("Username, password and enrollment number are required", "error");
    return;
  }

  try {
    const res = await fetch("/api/register", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        username,
        password,
        enrollmentNumber,
        fullName,
        rollNumber,
        semester: 3,
        branch: "Information Technology",
        section: "IT-B"
      })
    });
    const data = await res.json();
    if (!res.ok) throw new Error(data.error || "Registration failed");

    state.currentUser = data.user;
    sessionStorage.setItem("campusflow_user", JSON.stringify(data.user));
    onLoginSuccess(true);
    showToast("Account created successfully! Welcome to IT-B Portal.", "success");
  } catch (err) {
    showToast(err.message, "error");
  }
}

async function handleRegisterModalSubmit(e) {
  e.preventDefault();
  const username = document.getElementById("modal-reg-username").value.trim();
  const password = document.getElementById("modal-reg-password").value.trim();
  const enrollmentNumber = document.getElementById("modal-reg-enr").value.trim();
  const fullName = document.getElementById("modal-reg-name").value.trim() || username;
  const rollNumber = document.getElementById("modal-reg-roll").value.trim();

  if (!username || !password || !enrollmentNumber) {
    showToast("Username, password and enrollment number are required", "error");
    return;
  }

  try {
    const res = await fetch("/api/register", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        username,
        password,
        enrollmentNumber,
        fullName,
        rollNumber,
        semester: 3,
        branch: "Information Technology",
        section: "IT-B"
      })
    });
    const data = await res.json();
    if (!res.ok) throw new Error(data.error || "Registration failed");

    showToast(`Classmate account '${username}' registered successfully!`, "success");
    el.modalRegisterStudent.classList.add("hidden");
    document.getElementById("form-register-modal").reset();
  } catch (err) {
    showToast(err.message, "error");
  }
}

function handleLogout() {
  state.currentUser = null;
  sessionStorage.removeItem("campusflow_user");
  showToast("Logged out safely. Your profile is secure.", "success");
  showAuthScreen();
}

// ============================================================================
// Event Listeners Setup
// ============================================================================
function setupEventListeners() {
  // Auth screen tab switcher
  if (el.tabBtnSignin && el.tabBtnRegister) {
    el.tabBtnSignin.addEventListener("click", () => {
      el.tabBtnSignin.classList.add("active");
      el.tabBtnRegister.classList.remove("active");
      el.formLogin.classList.remove("hidden");
      el.formRegisterAuth.classList.add("hidden");
    });

    el.tabBtnRegister.addEventListener("click", () => {
      el.tabBtnRegister.classList.add("active");
      el.tabBtnSignin.classList.remove("active");
      el.formLogin.classList.add("hidden");
      el.formRegisterAuth.classList.remove("hidden");
    });
  }

  // Login form
  el.formLogin.addEventListener("submit", handleLoginSubmit);

  // Register form on auth screen
  if (el.formRegisterAuth) {
    el.formRegisterAuth.addEventListener("submit", handleRegisterAuthSubmit);
  }

  // Logout
  el.btnLogout.addEventListener("click", handleLogout);

  // Profile Edit modal
  el.btnOpenEditProfile.addEventListener("click", openEditProfileModal);
  el.btnCloseProfileModal.addEventListener("click", () => el.modalEditProfile.classList.add("hidden"));
  el.btnCancelProfile.addEventListener("click", () => el.modalEditProfile.classList.add("hidden"));
  el.formEditProfile.addEventListener("submit", handleProfileUpdate);

  // Register Student modal (Dashboard)
  if (el.btnOpenRegisterModal) {
    el.btnOpenRegisterModal.addEventListener("click", () => el.modalRegisterStudent.classList.remove("hidden"));
  }
  if (el.btnCloseRegisterModal) {
    el.btnCloseRegisterModal.addEventListener("click", () => el.modalRegisterStudent.classList.add("hidden"));
  }
  if (el.btnCancelRegModal) {
    el.btnCancelRegModal.addEventListener("click", () => el.modalRegisterStudent.classList.add("hidden"));
  }
  if (el.formRegisterModal) {
    el.formRegisterModal.addEventListener("submit", handleRegisterModalSubmit);
  }

  // Add Custom Subject modal
  el.btnOpenAddSubject.addEventListener("click", () => el.modalAddSubject.classList.remove("hidden"));
  el.btnOpenAddSubjectCr.addEventListener("click", () => el.modalAddSubject.classList.remove("hidden"));
  el.btnCloseSubjectModal.addEventListener("click", () => el.modalAddSubject.classList.add("hidden"));
  el.btnCancelSubject.addEventListener("click", () => el.modalAddSubject.classList.add("hidden"));
  el.formAddSubject.addEventListener("submit", handleAddCustomSubject);

  // Tab switching
  document.querySelectorAll(".tab-bar").forEach(bar => {
    bar.addEventListener("click", e => {
      const btn = e.target.closest(".tab-btn");
      if (!btn) return;
      const targetPaneId = "tab-" + btn.dataset.tab;
      
      bar.querySelectorAll(".tab-btn").forEach(b => b.classList.remove("active"));
      btn.classList.add("active");

      const parentSection = bar.closest(".dashboard-section");
      parentSection.querySelectorAll(".tab-pane").forEach(pane => pane.classList.remove("active"));
      const targetPane = document.getElementById(targetPaneId);
      if (targetPane) targetPane.classList.add("active");
    });
  });

  // Filters for assignments
  el.assignmentSearch.addEventListener("input", renderAssignments);
  el.filterSubject.addEventListener("change", renderAssignments);
  el.filterMedium.addEventListener("change", renderAssignments);

  // Date pickers for attendance
  el.attendanceDateInput.addEventListener("change", e => onAttendanceDateChange(e.target.value, false));
  el.crAttendanceDateInput.addEventListener("change", e => onAttendanceDateChange(e.target.value, true));

  // CR Actions
  el.btnOpenAddAssignment.addEventListener("click", () => {
    el.asgnAssignedDate.value = state.meta.todayDate;
    el.modalAddAssignment.classList.remove("hidden");
  });
  el.btnCloseAsgnModal.addEventListener("click", () => el.modalAddAssignment.classList.add("hidden"));
  el.btnCancelAsgn.addEventListener("click", () => el.modalAddAssignment.classList.add("hidden"));
  el.formAddAssignment.addEventListener("submit", handleAddAssignment);

  el.formAddHoliday.addEventListener("submit", handleAddHoliday);
}

// ============================================================================
// Member 1: Profile Editing
// ============================================================================
function openEditProfileModal() {
  if (!state.currentUser) return;
  document.getElementById("edit-fullname").value = state.currentUser.fullName || "";
  document.getElementById("edit-rollno").value = state.currentUser.rollNumber || "";
  document.getElementById("edit-enrno").value = state.currentUser.enrollmentNumber || "";
  document.getElementById("edit-semester").value = state.currentUser.semester || 3;
  document.getElementById("edit-section").value = state.currentUser.section || "IT-B";
  document.getElementById("edit-branch").value = state.currentUser.branch || "Information Technology";

  el.modalEditProfile.classList.remove("hidden");
}

async function handleProfileUpdate(e) {
  e.preventDefault();
  const payload = {
    userId: state.currentUser.id,
    fullName: document.getElementById("edit-fullname").value.trim(),
    rollNumber: document.getElementById("edit-rollno").value.trim(),
    enrollmentNumber: document.getElementById("edit-enrno").value.trim(),
    semester: parseInt(document.getElementById("edit-semester").value) || 3,
    branch: document.getElementById("edit-branch").value.trim(),
    section: document.getElementById("edit-section").value.trim()
  };

  try {
    const res = await fetch("/api/profile/update", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload)
    });
    const data = await res.json();
    if (!res.ok) throw new Error(data.error || "Failed to update profile");

    state.currentUser = data.user;
    sessionStorage.setItem("campusflow_user", JSON.stringify(data.user));
    
    updateNavProfileUI();
    if (state.currentUser.role === "CR") {
      el.crWelcomeName.textContent = state.currentUser.fullName;
    } else {
      updateStudentWelcomeBanner();
    }

    el.modalEditProfile.classList.add("hidden");
    showToast("Profile details updated successfully!", "success");
  } catch (err) {
    showToast(err.message, "error");
  }
}

// ============================================================================
// Data Fetching & Sync
// ============================================================================
async function loadMeta() {
  try {
    const res = await fetch("/api/meta");
    if (!res.ok) throw new Error("Meta fetch failed");
    const data = await res.json();
    state.meta = data;

    const dateObj = new Date(data.todayDate + "T00:00:00");
    const dateFormatted = dateObj.toLocaleDateString("en-US", {
      weekday: "short",
      year: "numeric",
      month: "short",
      day: "numeric"
    });
    el.todayDatePill.textContent = `${dateFormatted}`;

    state.selectedDate = data.todayDate;
    el.attendanceDateInput.value = data.todayDate;
    el.attendanceDateInput.max = data.todayDate;
    el.crAttendanceDateInput.value = data.todayDate;
    el.crAttendanceDateInput.max = data.todayDate;
  } catch (err) {
    state.meta.todayDate = new Date().toISOString().split("T")[0];
    state.selectedDate = state.meta.todayDate;
  }
}

async function refreshAllUserData() {
  if (!state.currentUser) return;
  await Promise.all([
    fetchStudentSubjects(),
    fetchAssignments(),
    fetchHolidays(),
    fetchAttendance()
  ]);
  checkDateRestrictions(state.selectedDate, false);
  checkDateRestrictions(state.selectedDate, true);
  renderDailySubjectMarking(false);
  renderDailySubjectMarking(true);
}

// Background sync to catch CR changes in real-time
async function syncRealtimeData() {
  if (!state.currentUser) return;
  try {
    const [asgnRes, holRes] = await Promise.all([
      fetch("/api/assignments"),
      fetch("/api/holidays")
    ]);
    if (asgnRes.ok) {
      const newAsgns = await asgnRes.json();
      if (JSON.stringify(newAsgns) !== JSON.stringify(state.assignments)) {
        state.assignments = newAsgns;
        renderAssignments();
        renderCrAssignmentsTable();
      }
    }
    if (holRes.ok) {
      const newHols = await holRes.json();
      if (JSON.stringify(newHols) !== JSON.stringify(state.holidays)) {
        state.holidays = newHols;
        renderHolidaysList();
        checkDateRestrictions(state.selectedDate, false);
        checkDateRestrictions(state.selectedDate, true);
        renderDailySubjectMarking(false);
        renderDailySubjectMarking(true);
      }
    }
  } catch (err) {
    // Silent background sync fail
  }
}

async function fetchStudentSubjects() {
  if (!state.currentUser) return;
  try {
    const res = await fetch(`/api/subjects?studentId=${state.currentUser.id}`);
    if (res.ok) {
      state.studentSubjects = await res.json();
      populateSubjectDropdowns(state.studentSubjects);
    }
  } catch (err) {
    console.error(err);
  }
}

function populateSubjectDropdowns(subjects) {
  el.filterSubject.innerHTML = '<option value="ALL">All Subjects</option>';
  subjects.forEach(sub => {
    const opt = document.createElement("option");
    opt.value = sub;
    opt.textContent = sub;
    el.filterSubject.appendChild(opt);
  });
}

async function fetchAssignments() {
  try {
    const res = await fetch("/api/assignments");
    if (!res.ok) throw new Error("Failed to load assignments");
    state.assignments = await res.json();
  } catch (err) {
    console.error(err);
  }
  renderAssignments();
  renderCrAssignmentsTable();
}

async function fetchHolidays() {
  try {
    const res = await fetch("/api/holidays");
    if (!res.ok) throw new Error("Failed to load holidays");
    state.holidays = await res.json();
  } catch (err) {
    console.error(err);
  }
  renderHolidaysList();
}

async function fetchAttendance() {
  if (!state.currentUser) return;
  try {
    const res = await fetch(`/api/attendance?studentId=${state.currentUser.id}`);
    if (!res.ok) throw new Error("Failed to load attendance");
    state.attendance = await res.json();
  } catch (err) {
    console.error(err);
  }
  renderAttendanceAnalytics(false);
  if (state.currentUser.role === "CR") {
    renderAttendanceAnalytics(true);
  }
}

// ============================================================================
// Member 3: Custom Subjects Addition
// ============================================================================
async function handleAddCustomSubject(e) {
  e.preventDefault();
  const subjectName = document.getElementById("custom-subject-name").value.trim();
  if (!subjectName) return;

  try {
    const res = await fetch("/api/subjects/add", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        studentId: state.currentUser.id,
        subjectName: subjectName
      })
    });
    const data = await res.json();
    if (!res.ok) throw new Error(data.error || "Failed to add subject");

    showToast(`'${subjectName}' added to your curriculum!`, "success");
    el.modalAddSubject.classList.add("hidden");
    document.getElementById("custom-subject-name").value = "";

    await fetchStudentSubjects();
    await fetchAttendance();
    renderDailySubjectMarking(false);
    renderDailySubjectMarking(true);
  } catch (err) {
    showToast(err.message, "error");
  }
}

// ============================================================================
// Member 2: Assignment Hub
// ============================================================================
function getMediumBadgeClass(medium) {
  const m = (medium || "").toLowerCase();
  if (m.includes("practical") || m.includes("lab")) return "practical-copy";
  if (m.includes("stick")) return "stick-file";
  if (m.includes("graph") || m.includes("loose")) return "loose-sheets";
  if (m.includes("spiral")) return "spiral-file";
  return "assignment-copy";
}

function getMediumIcon(medium) {
  const m = (medium || "").toLowerCase();
  if (m.includes("practical") || m.includes("lab")) return "📓";
  if (m.includes("stick")) return "📁";
  if (m.includes("graph") || m.includes("loose")) return "📄";
  if (m.includes("spiral")) return "🌀";
  return "📝";
}

function renderAssignments() {
  const query = el.assignmentSearch.value.trim().toLowerCase();
  const subjectFilter = el.filterSubject.value;
  const mediumFilter = el.filterMedium.value;

  const filtered = state.assignments.filter(a => {
    const matchesQuery = !query || 
      a.title.toLowerCase().includes(query) ||
      a.subjectName.toLowerCase().includes(query) ||
      a.facultyName.toLowerCase().includes(query) ||
      a.submissionMedium.toLowerCase().includes(query);

    const matchesSubject = (subjectFilter === "ALL") || (a.subjectName === subjectFilter);
    const matchesMedium = (mediumFilter === "ALL") || (a.submissionMedium.toLowerCase().includes(mediumFilter.toLowerCase()));

    return matchesQuery && matchesSubject && matchesMedium;
  });

  if (filtered.length === 0) {
    el.assignmentsContainer.innerHTML = `
      <div style="grid-column: 1 / -1; text-align: center; padding: 48px 20px; color: var(--text-muted);">
        <p style="font-size: 1.1rem; margin-bottom: 6px;">🍃 No assignments match your criteria.</p>
        <p style="font-size: 0.85rem;">Check other subjects or submission mediums.</p>
      </div>
    `;
    return;
  }

  el.assignmentsContainer.innerHTML = filtered.map(a => {
    const isDone = state.doneAssignments.has(a.id);
    const mediumClass = getMediumBadgeClass(a.submissionMedium);
    const mediumIcon = getMediumIcon(a.submissionMedium);

    let daysRemainingText = "";
    if (a.deadlineDate) {
      const today = new Date(state.meta.todayDate || new Date().toISOString().split("T")[0]);
      const deadline = new Date(a.deadlineDate);
      const diffDays = Math.ceil((deadline - today) / (1000 * 60 * 60 * 24));
      if (diffDays < 0) {
        daysRemainingText = `<span style="color: var(--accent-rose); font-weight: 700;">Overdue by ${Math.abs(diffDays)}d</span>`;
      } else if (diffDays === 0) {
        daysRemainingText = `<span style="color: var(--accent-amber); font-weight: 700;">Due Today</span>`;
      } else if (diffDays === 1) {
        daysRemainingText = `<span style="color: var(--accent-amber); font-weight: 700;">Due Tomorrow</span>`;
      } else {
        daysRemainingText = `<span>${diffDays} days left</span>`;
      }
    }

    return `
      <div class="assignment-card ${isDone ? 'is-completed' : ''}">
        <div class="card-top-badges">
          <span class="subject-code-pill">${escapeHtml(a.subjectCode || 'IT301')}</span>
          <span class="medium-pill ${mediumClass}">
            ${mediumIcon} ${escapeHtml(a.submissionMedium)}
          </span>
        </div>
        
        <div class="asgn-subject-title">${escapeHtml(a.subjectName)}</div>
        <h4 class="asgn-title">${escapeHtml(a.title)}</h4>
        <p class="asgn-desc">${escapeHtml(a.description || 'Complete and submit on time.')}</p>

        <div class="asgn-meta-dates">
          <div class="date-col">
            <span class="label">Assigned</span>
            <span class="val">${escapeHtml(a.assignedDate)}</span>
          </div>
          <div class="date-col deadline">
            <span class="label">Deadline</span>
            <span class="val">${escapeHtml(a.deadlineDate)}</span>
          </div>
          <div class="date-col">
            <span class="label">Status</span>
            <span class="val">${daysRemainingText}</span>
          </div>
        </div>

        <div class="asgn-footer-info">
          <span>Faculty: <strong>${escapeHtml(a.facultyName)}</strong></span>
          <label class="asgn-completed-label">
            <input type="checkbox" ${isDone ? 'checked' : ''} onchange="toggleAssignmentDone('${a.id}')">
            <span>${isDone ? 'Done ✓' : 'Mark Done'}</span>
          </label>
        </div>
      </div>
    `;
  }).join("");
}

function toggleAssignmentDone(id) {
  if (state.doneAssignments.has(id)) {
    state.doneAssignments.delete(id);
    showToast("Assignment marked as pending", "success");
  } else {
    state.doneAssignments.add(id);
    showToast("Assignment marked as completed ✓", "success");
  }
  localStorage.setItem("campusflow_done", JSON.stringify(Array.from(state.doneAssignments)));
  renderAssignments();
}

function renderCrAssignmentsTable() {
  if (!el.crAssignmentsTbody) return;
  if (state.assignments.length === 0) {
    el.crAssignmentsTbody.innerHTML = `<tr><td colspan="7" style="text-align:center; padding: 24px;">No assignments published yet.</td></tr>`;
    return;
  }

  el.crAssignmentsTbody.innerHTML = state.assignments.map(a => {
    return `
      <tr>
        <td><strong>${escapeHtml(a.subjectName)}</strong><br><small style="color:var(--text-muted);">${escapeHtml(a.subjectCode)}</small></td>
        <td>${escapeHtml(a.title)}</td>
        <td><span class="medium-pill ${getMediumBadgeClass(a.submissionMedium)}">${getMediumIcon(a.submissionMedium)} ${escapeHtml(a.submissionMedium)}</span></td>
        <td>${escapeHtml(a.assignedDate)}</td>
        <td><strong>${escapeHtml(a.deadlineDate)}</strong></td>
        <td>${escapeHtml(a.facultyName)}</td>
        <td>
          <button class="btn-delete-sm" onclick="handleDeleteAssignment('${a.id}')">Delete</button>
        </td>
      </tr>
    `;
  }).join("");
}

async function handleAddAssignment(e) {
  e.preventDefault();
  const payload = {
    subjectCode: document.getElementById("asgn-subject-code").value.trim(),
    subjectName: document.getElementById("asgn-subject-name").value.trim(),
    title: document.getElementById("asgn-title").value.trim(),
    submissionMedium: document.getElementById("asgn-medium").value,
    assignedDate: document.getElementById("asgn-assigned-date").value,
    deadlineDate: document.getElementById("asgn-deadline-date").value,
    facultyName: document.getElementById("asgn-faculty").value.trim() || "Faculty",
    maxMarks: parseInt(document.getElementById("asgn-marks").value) || 20,
    description: document.getElementById("asgn-description").value.trim(),
    instructions: document.getElementById("asgn-instructions").value.trim()
  };

  try {
    const res = await fetch("/api/assignments", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload)
    });
    const data = await res.json();
    if (!res.ok) throw new Error(data.error || "Failed to add assignment");

    showToast("Assignment published to IT-B classmates!", "success");
    el.modalAddAssignment.classList.add("hidden");
    el.formAddAssignment.reset();
    await fetchAssignments();
  } catch (err) {
    showToast(err.message, "error");
  }
}

async function handleDeleteAssignment(id) {
  if (!confirm("Are you sure you want to delete this assignment?")) return;
  try {
    const res = await fetch("/api/assignments/delete", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ id })
    });
    const data = await res.json();
    if (!res.ok) throw new Error(data.error || "Failed to delete assignment");

    showToast("Assignment deleted", "success");
    await fetchAssignments();
  } catch (err) {
    showToast(err.message, "error");
  }
}

// ============================================================================
// Member 3: Attendance, Restrictions & Safe Miss Calculator
// ============================================================================
function onAttendanceDateChange(dateVal, isCr = false) {
  state.selectedDate = dateVal;
  checkDateRestrictions(dateVal, isCr);
  renderDailySubjectMarking(isCr);
}

function checkDateRestrictions(dateStr, isCr = false) {
  const noticeBox = isCr ? el.crDateStatusNotice : el.dateStatusNotice;
  if (!noticeBox) return { isLocked: true };

  if (!dateStr) return { isLocked: true };

  const today = state.meta.todayDate || new Date().toISOString().split("T")[0];

  // 1. Future date check
  if (dateStr > today) {
    noticeBox.className = "status-notice-box locked-future";
    noticeBox.innerHTML = `<span>⏳ <strong>Future Date Locked:</strong> You cannot mark attendance on upcoming dates (${dateStr}).</span>`;
    return { isLocked: true, reason: "Future Date" };
  }

  // 2. Weekend check (Saturday / Sunday)
  const dateObj = new Date(dateStr + "T00:00:00");
  const day = dateObj.getDay();
  if (day === 0 || day === 6) {
    const dayName = day === 0 ? "Sunday" : "Saturday";
    noticeBox.className = "status-notice-box locked-weekend";
    noticeBox.innerHTML = `<span>🌴 <strong>Fixed Weekend Holiday:</strong> ${dateStr} is ${dayName}. Saturday & Sunday are non-instructional days. Attendance is disabled.</span>`;
    return { isLocked: true, reason: "Weekend" };
  }

  // 3. CR-declared holiday check
  const matchedHoliday = state.holidays.find(h => h.date === dateStr);
  if (matchedHoliday) {
    noticeBox.className = "status-notice-box locked-holiday";
    noticeBox.innerHTML = `<span>🎉 <strong>Official Holiday:</strong> ${dateStr} is off for <em>"${escapeHtml(matchedHoliday.description)}"</em> (Declared by ${escapeHtml(matchedHoliday.declaredBy || 'CR')}).</span>`;
    return { isLocked: true, reason: "College Holiday" };
  }

  noticeBox.className = "status-notice-box hidden";
  return { isLocked: false };
}

function renderDailySubjectMarking(isCr = false) {
  const restriction = checkDateRestrictions(state.selectedDate, isCr);
  const container = isCr ? el.crDailySubjectMarkingList : el.dailySubjectMarkingList;
  if (!container) return;

  const recordsForDate = (state.attendance.history || []).filter(r => r.date === state.selectedDate);
  const statusMap = {};
  recordsForDate.forEach(r => { statusMap[r.subjectName] = r.status; });

  const subjects = state.studentSubjects.length > 0 ? state.studentSubjects : (state.meta.subjects || []);

  if (subjects.length === 0) {
    container.innerHTML = `<p style="color:var(--text-muted); padding:16px;">Loading subjects...</p>`;
    return;
  }

  container.innerHTML = subjects.map(sub => {
    const currentStatus = statusMap[sub] || "";
    const isLocked = restriction.isLocked;
    const isCustom = !state.meta.subjects.includes(sub);

    return `
      <div class="marking-item-row">
        <div class="subject-info-title">
          <span>${escapeHtml(sub)}</span>
          ${isCustom ? '<span class="custom-tag">Custom Subject</span>' : ''}
        </div>
        <div class="marking-actions">
          <button class="btn-mark present ${currentStatus === 'PRESENT' ? 'active' : ''}" 
                  ${isLocked ? 'disabled style="opacity:0.4; cursor:not-allowed;"' : ''}
                  onclick="markAttendance('${escapeAttr(sub)}', 'PRESENT', ${isCr})">
            Present
          </button>
          <button class="btn-mark absent ${currentStatus === 'ABSENT' ? 'active' : ''}" 
                  ${isLocked ? 'disabled style="opacity:0.4; cursor:not-allowed;"' : ''}
                  onclick="markAttendance('${escapeAttr(sub)}', 'ABSENT', ${isCr})">
            Absent
          </button>
          <button class="btn-mark cancelled ${currentStatus === 'CANCELLED' ? 'active' : ''}" 
                  ${isLocked ? 'disabled style="opacity:0.4; cursor:not-allowed;"' : ''}
                  onclick="markAttendance('${escapeAttr(sub)}', 'CANCELLED', ${isCr})">
            Class Cancelled
          </button>
        </div>
      </div>
    `;
  }).join("");
}

async function markAttendance(subjectName, status, isCr = false) {
  try {
    const payload = {
      studentId: state.currentUser.id,
      subjectName: subjectName,
      date: state.selectedDate,
      status: status
    };

    const res = await fetch("/api/attendance", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload)
    });

    const data = await res.json();
    if (!res.ok) {
      throw new Error(data.error || "Failed to mark attendance");
    }

    showToast(`Marked ${status} for ${subjectName}`, "success");
    await fetchAttendance();
    renderDailySubjectMarking(isCr);
  } catch (err) {
    showToast(err.message, "error");
  }
}

function renderAttendanceAnalytics(isCr = false) {
  const stats = state.attendance.subjectStats || [];
  const container = isCr ? el.crSubjectsAnalyticsGrid : el.subjectsAnalyticsGrid;
  if (!container) return;

  if (!isCr) {
    const overall = state.attendance.overallPercentage || 0;
    el.overallPercentageVal.textContent = `${overall.toFixed(1)}%`;
    if (overall >= 75) {
      el.overallStatusPill.className = "mini-status-pill safe";
      el.overallStatusPill.textContent = "Safe (≥ 75%)";
    } else if (overall >= 65) {
      el.overallStatusPill.className = "mini-status-pill warning";
      el.overallStatusPill.textContent = "Warning (65% - 74%)";
    } else {
      el.overallStatusPill.className = "mini-status-pill critical";
      el.overallStatusPill.textContent = "Critical (< 65%)";
    }
  }

  if (stats.length === 0) {
    container.innerHTML = `
      <div style="grid-column: 1 / -1; text-align: center; padding: 32px; color: var(--text-muted);">
        No attendance records found yet.
      </div>
    `;
    return;
  }

  container.innerHTML = stats.map(s => {
    const pct = s.percentage || 0;
    let badgeClass = "safe";
    if (pct < 65) badgeClass = "critical";
    else if (pct < 75) badgeClass = "warning";

    let target75Desc = "";
    if (pct >= 75) {
      target75Desc = `<span class="target-prediction safe">🌴 You can safely bunk <strong>${s.safeMisses75}</strong> class(es)</span>`;
    } else {
      target75Desc = `<span class="target-prediction danger">⚠️ Attend next <strong>${s.requiredAttend75}</strong> class(es) consecutively</span>`;
    }

    let target65Desc = "";
    if (pct >= 65) {
      target65Desc = `<span class="target-prediction safe">🌴 You can safely bunk <strong>${s.safeMisses65}</strong> class(es)</span>`;
    } else {
      target65Desc = `<span class="target-prediction danger">🚨 Attend next <strong>${s.requiredAttend65}</strong> class(es) to avoid debarment</span>`;
    }

    return `
      <div class="analytics-card">
        <div class="analytics-card-top">
          <h4 class="subject-card-title">${escapeHtml(s.subjectName)}</h4>
          <span class="percentage-badge ${badgeClass}">${pct.toFixed(1)}%</span>
        </div>

        <div class="stats-count-sub">
          Attended: <strong>${s.attendedCount}</strong> of <strong>${s.totalHeld}</strong> held
          ${s.cancelledCount > 0 ? `• (${s.cancelledCount} cancelled)` : ''}
        </div>

        <div class="progress-bar-wrap">
          <div class="progress-bar-fill ${badgeClass}" style="width: ${Math.min(100, Math.max(0, pct))}%"></div>
        </div>

        <div class="dual-target-box">
          <div class="target-row">
            <span class="target-label">Standard Goal (75%):</span>
            ${target75Desc}
          </div>
          <div class="target-row">
            <span class="target-label">Medical Limit (65%):</span>
            ${target65Desc}
          </div>
        </div>
      </div>
    `;
  }).join("");
}

// ============================================================================
// CR Holiday Actions
// ============================================================================
function renderHolidaysList() {
  if (!el.holidaysListContainer) return;
  if (state.holidays.length === 0) {
    el.holidaysListContainer.innerHTML = `<p style="color:var(--text-muted); padding:16px;">No custom holidays declared yet.</p>`;
    return;
  }

  el.holidaysListContainer.innerHTML = state.holidays.map(h => {
    return `
      <div class="holiday-item">
        <div>
          <span class="holiday-date-badge">${escapeHtml(h.date)}</span>
          <div class="holiday-reason">${escapeHtml(h.description)}</div>
          <small style="color:var(--text-muted);">Declared by: ${escapeHtml(h.declaredBy || 'CR')}</small>
        </div>
        <button class="btn-delete-sm" onclick="handleDeleteHoliday('${h.date}')">Remove</button>
      </div>
    `;
  }).join("");
}

async function handleAddHoliday(e) {
  e.preventDefault();
  const date = el.holidayDateInput.value;
  const description = document.getElementById("holiday-desc").value.trim();

  try {
    const res = await fetch("/api/holidays", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        date,
        description,
        declaredBy: state.currentUser.fullName
      })
    });
    const data = await res.json();
    if (!res.ok) throw new Error(data.error || "Failed to declare holiday");

    showToast("Holiday declared! Classmate calendars updated.", "success");
    el.formAddHoliday.reset();
    await fetchHolidays();
    checkDateRestrictions(state.selectedDate, false);
    checkDateRestrictions(state.selectedDate, true);
    renderDailySubjectMarking(false);
    renderDailySubjectMarking(true);
  } catch (err) {
    showToast(err.message, "error");
  }
}

async function handleDeleteHoliday(date) {
  if (!confirm(`Remove holiday for ${date}?`)) return;
  try {
    const res = await fetch("/api/holidays/delete", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ date })
    });
    const data = await res.json();
    if (!res.ok) throw new Error(data.error || "Failed to remove holiday");

    showToast("Holiday removed", "success");
    await fetchHolidays();
    checkDateRestrictions(state.selectedDate, false);
    checkDateRestrictions(state.selectedDate, true);
    renderDailySubjectMarking(false);
    renderDailySubjectMarking(true);
  } catch (err) {
    showToast(err.message, "error");
  }
}

// ============================================================================
// UI Toast Helpers
// ============================================================================
function showToast(message, type = "success") {
  const toast = document.createElement("div");
  toast.className = `toast ${type}`;
  toast.innerHTML = `
    <span>${type === 'success' ? '✓' : '⚠️'}</span>
    <span>${escapeHtml(message)}</span>
  `;
  el.toastContainer.appendChild(toast);

  setTimeout(() => {
    toast.style.opacity = "0";
    toast.style.transform = "translateX(20px)";
    toast.style.transition = "all 0.25s ease-out";
    setTimeout(() => toast.remove(), 250);
  }, 3800);
}

function escapeHtml(str) {
  if (!str) return "";
  return String(str)
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#039;");
}

function escapeAttr(str) {
  if (!str) return "";
  return String(str).replace(/'/g, "\\'");
}

// Start application
window.addEventListener("DOMContentLoaded", init);

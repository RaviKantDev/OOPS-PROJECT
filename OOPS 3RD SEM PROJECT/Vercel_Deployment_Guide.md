# 🚀 How to Deploy CampusFlow to Vercel (Live Cloud URL)

Your project is already **100% pre-configured for Vercel** with [`vercel.json`](file:///d:/OOPS%203RD%20SEM%20PROJECT/vercel.json) and [`api/index.js`](file:///d:/OOPS%203RD%20SEM%20PROJECT/api/index.js)!

Follow these 3 simple steps to get a live URL (e.g., `https://campusflow-it-b.vercel.app`) that you can share with your teacher and classmates:

---

## 📌 Step 1: Upload Your Code to GitHub

1. Open PowerShell or Terminal in your project directory (`D:\OOPS 3RD SEM PROJECT`).
2. Run these commands:
   ```bash
   git init
   git add .
   git commit -m "Initial commit of CampusFlow IT-B Portal"
   ```
3. Go to [github.com](https://github.com) and create a new repository named:
   **`campusflow-it-b`** (Set to Public).
4. Link your local project to GitHub and push:
   ```bash
   git branch -M main
   git remote add origin https://github.com/YOUR_GITHUB_USERNAME/campusflow-it-b.git
   git push -u origin main
   ```

---

## 📌 Step 2: Deploy on Vercel (In 1 Click)

1. Go to **[vercel.com](https://vercel.com)** and sign in with your GitHub account (it's completely free).
2. On your Vercel Dashboard, click the blue button: **"Add New..."** ➔ **"Project"**.
3. You will see your GitHub repository `campusflow-it-b`. Click **"Import"**.
4. **Leave all settings as default!**
   * *Framework Preset:* Other
   * *Root Directory:* `./`
   * *(The pre-configured `vercel.json` automatically links both the frontend and the cloud API)*.
5. Click **"Deploy"**!

---

## 📌 Step 3: Your Live Website is Ready! 🎉

In about 25 seconds, Vercel will complete the build and give you a live production link:
👉 **`https://campusflow-it-b.vercel.app`**

### What works on the live Vercel cloud link:
* **Student Registration:** Any classmate can open the link on their mobile phone or PC, click **"Register New Student"**, enter their username, password, and enrollment number, and create an account!
* **Default Logins Available:**
  * **CR:** `priyanshu_cr` / `priyanshu123`
  * **Classmate 1:** `prakash` / `prakash123`
  * **Classmate 2:** `ravikant` / `ravi123`
* **Real-time Attendance & Bunk Predictions:** Automatically calculates safe misses for $\ge 75\%$ and $\ge 65\%$ on working days.
* **Assignments & Submission Mediums:** View, filter, and track all IT-B assignments.

---

## 💡 How to Explain This to Your Professor

If your professor asks:
* **"Is this project built in C++?"**
  * *Answer:* "Yes, sir! Our core desktop application runs on a native C++ Object-Oriented engine (`server.exe` using Winsock sockets and C++ STL). For demonstration over the web on phones and laptops, we also deployed a cloud serverless bridge to Vercel so that our classmates can access it anywhere!"
* You can run [`run_cli.bat`](file:///d:/OOPS%203RD%20SEM%20PROJECT/run_cli.bat) on your laptop in lab to demonstrate the C++ classes and virtual functions directly in the terminal, while showing the live Vercel website on your phone!


# Student Performance Report System in C

## Overview
This C program allows users to input student academic data and generates a **student performance report**. It calculates total marks, average marks, assigns grades, and displays a visual performance representation using stars. The program also prints all student roll numbers recursively.

---

## Features
- Input details for multiple students:
  - Roll number
  - Name
  - Marks for 3 subjects
- Automatically calculates:
  - Total marks
  - Average marks
  - Grade (A, B, C, D, F)
- Displays performance using star symbols for grades A-D
- Prints all student roll numbers using **recursion**
- Handles up to 100 students (configurable via `MAX_NUM_STUDENTS`)

---

## Input Format

First line : Number of students (e.g., 2)
Next lines : Roll_Number Name Marks1 Marks2 Marks3 for each student
Example:
2
1 Ankit 83 79 88
2 Tanya 94 96 90

## Output Format
========= STUDENT PERFORMANCE REPORT =========

Roll : 1
Name : Ankit
Total : 250
Average : 83.33
Grade : B
Performance: ****

Roll : 2
Name : Tanya
Total : 280
Average : 93.33
Grade : A
Performance: *****

List of Roll Numbers (via recursion): 1 2

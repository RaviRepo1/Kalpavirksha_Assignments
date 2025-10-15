# 🧮 Sonar Image Processing Matrix Operations in C

This C program performs **matrix operations** such as rotation and smoothing on a square matrix.  
You can either **input a matrix manually** or **generate a random matrix** with values between 0 and 255.  

---

## 📘 Description

The program performs the following steps:

1. Takes the size of a square matrix (**N x N**, where 2 ≤ N ≤ 10).  
2. Allows the user to **either input elements manually** or **generate them randomly**.  
3. Displays the **original matrix**.  
4. **Rotates** the matrix **90° clockwise**.  
5. Applies a **smoothing filter**, replacing each element with the average of itself and its neighboring elements.  
6. Displays all results step by step.  

---

## ✨ Features

- Supports matrix sizes from **2 to 10**.  
- Option to choose **random** or **user-input** matrix.  
- Performs **90° clockwise rotation**.  
- Applies a **smoothing filter** efficiently using **O(N)** extra space.  
- Handles invalid inputs gracefully.  

---

## The program prints:
1. **Original Matrix**  
2. **Matrix after 90° clockwise rotation**  
3. **Matrix after applying the smoothing filter**


## 🧩 Input Format by Example for Random Matrix
### ## 🧮 Example 1 — Random Matrix

Important:-  
For this, just uncomment this line in code.  
// generateRandomMatrix(ptr, n, MIN_RANDOM_SIZE, MAX_RANDOM_SIZE);

### Input:-  
Enter matrix size (2-10): 3

### Output:-  
Original Matrix (Random 0-255):<br>
169  22 112<br>
 47 122  50<br>
181 220  70<br>

Matrix after 90 degree Clockwise Rotation:<br>
181  47 169<br>
220 122  22<br>
 70  50 112<br>

Matrix after Applying Smoothing Filter: <br>  
142 126  90<br>
115 110  87<br>
115  99  76<br>


## 🧩 Input Format by Example for User Input Matrix
### Example 1 — User Matrix

Important:-  
For this, just uncomment this line in code.  
// inputMatrix(ptr, n);

### Input:-  
Enter matrix size (2-10): 3<br>
Enter 9 elements row by row:<br>
1 2 3<br>
4 5 6<br>
7 8 9<br>

### Output:-  

Original Matrix:<br>
1 2 3<br>
4 5 6<br>
7 8 9<br>

Matrix after 90 degree Clockwise Rotation:<br>
7 4 1<br>
8 5 2<br>
9 6 3<br>

Matrix after Applying Smoothing Filter:<br>
6 5 3<br>
7 5 3<br>
8 6 4<br>

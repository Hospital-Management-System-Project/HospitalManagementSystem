<h1 align="center"><b>Hospital Management System 🚑🏥</b></h1>

<p align="center"><b>
A C++-based multi-hospital network management application demonstrating OOP principles.
</b></p>

---

## 📖 Description

The **Hospital Management System** is a C++ application designed to coordinate patient care, staffing, billing, and transfers across multiple hospital locations. By leveraging **Object-Oriented Programming (OOP)** concepts, it provides a clear structure to manage a network of hospitals, doctors, nurses, and patients. Our system supports up to five hospitals, 50 doctors, 60 nurses, and 100 patients overall, with core functionalities including:

- Patient admission, discharge, and transfer.
- Doctor and nurse assignments with capacity constraints.
- Basic billing system based on daily rates.
- Extendable design for future integration with databases, GUIs, or pharmacies.

---

## Project Overview

This **Hospital Management System** focuses on:
- Managing **Patients** (admission, discharge, transfer, billing).
- Managing **Staff** (Doctors, Nurses) across multiple hospital locations.
- Demonstrating OOP principles (Encapsulation, Inheritance, Polymorphism, Composition).
- Providing a foundation for further **innovations** like database integration, UI/UX, or more sophisticated reporting modules.

**Why we built this**:  
> Hospital management involves complex workflows and large volumes of data. We wanted to simulate real-world constraints (limited beds, staff capacity, multiple locations) while maintaining a clear object-oriented design.

---

## 🚀 Key Features

- **Multi-Hospital Network**  
  Each hospital can admit up to 20 patients, allowing for a combined capacity of 100 across five locations.

- **Doctors & Nurses**  
  - Up to 50 doctors with unique IDs and specialties.  
  - Up to 60 nurses, each caring for a maximum of two patients.

- **Patient Management**  
  - Unique Patient IDs, personal details, disease information, and contact details.  
  - Automatic assignment of a primary doctor (plus additional doctors if needed).  
  - Transfer between hospitals without data loss.

- **Billing System**  
  - Basic daily-rate calculation; can be extended for complex billing rules.  
  - Integrated in each hospital to generate invoices.

- **Extendable Architecture**  
  - Optional `Pharmacy` module for prescription management and pharmacy billing.  
  - Room for additional features like real-time reporting, analytics, and user interfaces.

---

## Technologies Used

| Technology      | Purpose                                     |
|-----------------|---------------------------------------------|
| **C++17+**      | Core language for object-oriented design    |
| **CMake** (Opt) | Build system (if you choose to use it)      |
| **Git/GitHub**  | Version control, collaboration              |
| **(Optional)**  | **MySQL/SQLite** for database connectivity  |
| **(Optional)**  | **Qt/SFML** for GUI development             |

*This project is primarily built with standard C++ STL without extra libraries, making it easy to compile on most platforms.*

---

## 📜 License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

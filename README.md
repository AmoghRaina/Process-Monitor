

# Process-Monitor Application

## Overview

The Process Manager Application is a powerful tool designed to provide comprehensive process management capabilities on Windows. This application offers a user-friendly graphical interface for viewing and interacting with running processes. Key functionalities include displaying detailed process information, searching for specific processes, terminating processes, retrieving process descriptions, and accessing the file location of processes.




## Features

### Process Listing

-   **Process Name, PID, Architecture, Status, and Memory Usage**: Displays essential information about each running process, including its name, process ID (PID), architecture (x86/x64), current status (running or inactive), and memory usage.
-   **Dynamic Icons**: Each process is displayed with an icon representing its executable file, making it easier to identify processes visually.

### Process Search

-   **Live Search**: The search functionality allows users to quickly find processes by their name. As users type in the search box, the list of processes updates in real-time to match the search query.

### Context Menu Actions

-   **Terminate Process**: Right-clicking on a process opens a context menu where users can choose to terminate the selected process.
-   **Process Information**: Provides a detailed popup with all available information about the selected process.
-   **Open File Location**: Allows users to open the file location of the selected process in Windows Explorer.

### Menu Bar

-   **Hide Protected Processes**: A menu option to hide protected processes, making the process list cleaner and easier to navigate.

### Detailed Process Information

-   **Architecture Detection**: Identifies whether a process is running under x86 or x64 architecture.
-   **Status Detection**: Determines if a process is still active or has exited.
-   **Memory Usage**: Displays the memory usage of each process in a human-readable format (MB).
## Screenshots
<img src="https://github.com/user-attachments/assets/a73aae8d-1299-4b35-8ab9-4792b5e53d8f" alt="Process List" width="500"/>

## Technologies Used

-   **Qt**: Used for building the graphical user interface.
-   **Windows API**: Utilized for accessing process information and performing actions such as terminating processes and opening file locations.
-   **C++**: The core programming language for implementing the application's functionality.
## Future additions

1.  **Driver communication** (currently using system calls to retrieve process information) 
2.  **Cpu utilization** 
3. **Usage-Graph representation** 
4. **ProcessManipulators** 
5. **Inbuilt process offset manipulation** 
6. **Unwanted processes finder** 



## How to Use

1.  **Clone the Repository**: 
2.  **Build the Application**: Open the project in Qt 6.0 or higher and build the project.
3.  **Run the Application**: Execute the built application to start managing processes on your Windows system.


## Contributing

Contributions are welcome! If you have any ideas, suggestions, or bug reports, please open an issue or submit a pull request. Let's work together to improve this application.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.

## Contact

For any inquiries or support, please contact me at amoghraina2000@gmail.com.

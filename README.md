### FTP program with data compression
---
## What is it about?
> TBC

---
## Motivation:
FTP is a common network service. Since you have learned how to write UNIX socket
programs in class, this homework asks you to practice the socket programming skill.
## Homework Content:
You need to provide the FTP service in the homework. By using TCP sockets, you can
create a FTP server and a FTP client. Your FTP client has to support some commands:

&#10146; **link** IP_addr port_num:\
Connect to a server. Your program should allow users to specify the IP address and
port number of a server. Notice that the server side needs to print out the client’s
connection information (e.g., IP address, port #, socket type, etc.)

&#10146; **leave**:\
Terminate the connection. The server side has to also show the leave of the client.

&#10146; **send** filename:\
Upload a file called “filename” to the server. Your FTP service should also support
<u>non-textual files</u>, for example, MP3 or WORD files. Moreover, you need to provide file compression to save the transmission time. In particular, you can use <u>fixed-length Huffman coding</u> to compress the file. When you use the “send” command, your client in fact should send two separated files to the server:
  
  &#9332; The coded version of “filenanme”.\
  &#9333; Necessary information related to Huffman coding (e.g., the appearing frequency
of each alphabet in the file).

Here are some examples of the above commands:

| Client Side <140.113.1.1> | Server Side <140.113.1.2:1234> |
|---------------------------|---------------|
| [student @ CSE ~ ]$ link 140.113.1.2 3456 | [TA @ CSE ~] server # TA runs the server program  |
| The server with IP address “140.113.1.2” has accepted your connection. | A client “140.113.1.1” has connected via port num 1732 using SOCK_STREAM (TCP) |
| [student @ CSE ~]$ send 1.txt | The client sends a file “1.txt” with size of 140,234 bytes. The Huffman coding data are stored in “1-code.txt”. |
| Original file length: 140,234 bytes, compressed file length: 23,768 bytes (ratio: 16.95%) | # TA will open “1.txt” here to test its correctness. Also, TA will check the “1-code.txt” file  to make sure that you indeed compress the file via Huffman coding. |
| Time to upload: 2021/3/24 13:20 | The client “140.113.1.1” with port 4323 has terminated the connection |
| Using fixed-length codeword (3 bits) | |
| [student @ CSE ~]$ leave | |
| Bye bye. | |

Below gives some format of the Huffman coding-related information:

| Fixed-length Huffman coding (3-bit codeword) |                  |          |
|----------------------------------------------|------------------|----------|
| Alphabet (ASCII code)                        | Frequency(%)     | Codeword |
| A (65)                                       | 140,231 (21.45%) | 000      |
| B (66)                                       | 50,123 (7.67%)   | 001      |
| C (67)                                       | 63,154 (9.66%)   | 010      |
| …                                            |                  |          |
| Z (90)                                       | 234,453 (35.86%) | 011      |

## Requirements:
&#10146; You need to use <u>UNIX socket programming</u> in this homework.

&#10146; You have to provide a makefile. TAs will deduct your grades if there is no <u>makefile</u>, or
the makefile is erroneous.

&#10146; You must submit a README file along with your program. The README file should
briefly describe <u>how you write your codes</u> (for example, the idea of your program).

&#10146; You have to demonstrate your program. TAs will announce the demonstration time.

## Grading Policy:

You need to submit your codes and demonstrate your program to TAs. The due day of this homework is <span style="color:red">4/14</span>. You will get <span style="color:red">no point</span> if you do NOT demonstrate your program (even though you submit codes). Discussion among your classmates is encouraged. However, plagiarists will get <span style="color:red">ZERO point</span>. Below are the points you can get in this homework:
| Items | Points |
|-|-|
| Socket connection (i.e., link and leave commands on the client side and the connection information on the server side) | 10% |
| File transmission | 20% |
| Huffman coding | 50% |
| User interface (for example, welcome message or help command) | 10% |
| Code’s comments and README file | 10% |
| Bonus: You <span style="color:red">ALSO</span> provide variable-length Huffman coding. <br>[Note] If you implement only variable-length Huffman coding (without the fixed-length one), you will NOT get this bonus. | 20% |
<br>
> Reference: (Huffman coding) http://en.wikipedia.org/wiki/Huffman_coding 
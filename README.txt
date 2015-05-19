REMOTE CODE MANAGEMENT README:
---------------
Author: Gautam Nagesh Peri
email: nperi@syr.edu
Phone: 3154846562

1. Fetches the files on Server on Connect Click displayed on console.
2. File Upload (Select local file and click upload)
3. File Download (Hardocoded the File) please refer to Note below.
4. Search Text and Search in Files supports regular expression for files
5. The command Line arguments server:

<<SourcePort>> <<DestinationPort>> "<<FileName>>"

6. Client Listener Port is Hardcoded to 9081. Tried to change but issues were popping up.

Architecture
------------

GUI: Starts Receiver
     Sends the input to Sender via the MockChannel

Sender: Enqueues strings or stirng buffer to Blocking Queue
 deqThread: dequeues from Blocking queue to send it on socket.

Receiver: accepts connections
	client Handler () recived data from socket and enqueues to blocking queue
	 dequeue thread: dequeues from blockin queue and performs actions(file receive, recieve ack, recive string)


Note:
-----
Unable to bind the file listing data to the GUI hence displayed on the Console. As unable to bind data file download is hardcoded to 11245.txt

File transfers with exceeding 300 KB is failing.
Tried to fix, but unsuccessful, so small sized zip files are used for demonstration.

-Acknowledgements are replaced with functionalities.
please find it in \CppCli-WPF-App\Handlers\Handler.cpp    in FinishProcessing function
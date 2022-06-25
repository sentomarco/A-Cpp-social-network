# A C++ social network
  
  
**Author: Marco Sento**
  
This project consists in the implementation of an offline social network as requirements for the Algorithm Exam at Politecnico di Torino.  
This is the core of a social network, able to store many different information about the users with the minimum amount of resources, adopting an extensive use of the Standard Template Library in its 4000+ lines of code.  
The social network is able to manage three different typologies of users, performing almost 30 different operations that spans from users’ management to statistical data analysis.  
Many checks about the data inserted have to be performed in order to guarantee the integrity of the system.  

![immagine](https://user-images.githubusercontent.com/70527145/175788529-07d78abc-e3ac-48e4-99de-e79a98344c1e.png)
  
  
<h1> Social Network requirements: </h1>  
  
The goal is to realize a social network able to satisfy the following requests in the most possible flexible way.  
It must provide to the user the capability of:  

```
1)	View the ID of all the users, for all the types that are:  
1.1)	 Simple user (semplici): able to have friends, acquaintance, parents, partner, son, jobs and they can be part of some groups.  
They can also post messages and express like and dislike.  
1.2)	 Enterprise user (azienda): able to have employees and corporates, can also post messages and express like and dislike.  
1.3)	 Group user (gruppi): able to have subscribers.  
  
2)	View detailed information of each user:    
2.1) The ID, the name and surname, the b-day and all the additional info.  
2.2) The number of like and dislike expressed.  
2.3) The number of friends and acquaintances.  
2.4) The relationship status.  
2.5) The names of the parents and of the sons.  
2.6) The employee status.  
2.7) The ownership of a social network groups.  
  
3)	Manage individual users:   
3.1) Add, delete or change the information of a specific user on the base of its ID  
  
4)	Manage relationships between users:   
4.1) Add or remove a friend.  
4.2) Add or remove an acquaintance.  
4.3) Add or remove a parent.  
4.4) Add or remove a partner.  
4.5) Add or remove a son.  
4.6) Add or remove a job.  
4.7) Add or remove a group membership.  
  
5)	Manage messages and opinions:  
5.1) add or delete a message of a user.  
5.3) modify a message and its reactions of a user.  
5.4) view the messages of a user and their reactions.  
  
6)	View statistical functions about:  
6.1) Number of total users, number of simple users, company and group.  
6.2) Number of friends and relatives for each simple user.  
6.3) Number of users born after a specific date of birth.  
6.4) Number of employees and number of subsidiaries for each company.  
6.5) Number of users for each group.  
6.6) Company with the largest number of employees.  
6.7) News with the highest number of appreciations.  
6.8) News with the greatest number of indignations.  
6.9) User with the most friends.  
6.10) User with the most knowledge.  
6.11) Average age of simple users.  
   
7)	Perform researches:  
7.1) Get a user's family trees.  
7.2) Get family trees of all users  
7.3) Search for 'lone wolves', simple users who do not actively participate in social life.  
7.4) Sort companies by best ratio of positive and negative reactions to their news.  
7.5) Sort companies by worst ratio of positive and negative reactions to their news. 
```  
  
In doing this, continuously checks are performed on the format of the data provided to guarantee their validity.  
As input, 3 files must be provided, also empty, in order to store and acquire system’s data.  
The name are not relevant as the program is able to identify them on the base of their format, otherwise the name are requested to the user.  
The input files are:  
1)	A user file containing all the users in the system at the startup, provided in the following syntax:  
   
   ```
•	<ID>,utente semplice,{data di nascita:<gg-mm-yyyy>,Cognome:<surname>,Nome:<name>,<additional field>:<value>,....}  
•	<ID>,utente azienda,{Nome:<company name>,<additional field>:<value>,....}  
•	<ID>,utente gruppo,{Nome:<group name>,<additional field>:<value>,....}  
  ```
  
2)	A news file containing all the messages posted by the users, stored with the owner ID, the post time and the number of likes and dislikes, with the IDs of those like or dislike it.  
    
  ```
•	<ID>,<message>,data di nascita:<gg-mm-yyyy>,like:{ID1,..},dislike:{ID2,…}    
  ```
  
3)	A relations file that express the relationships between two user in the following form:  
  
  ```
•	<ID1>,<ID2>,<relationships>   
  ```
  
The reserved character are: { } , :  
Every user must have a unique ID, composed of an arbitrary sequence of characters.  
All the modifications update these files. 
  
  
  docs/Description.docx
  

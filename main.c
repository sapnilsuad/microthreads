// Header files here
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Preprocessor Macro for using colored text in terminal 
#define RESET   "\x1b[0m"
#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"

// Structure of Users information
struct User {
    char username[50];
    char password[50];
};
// Structure for Post details
struct Post {
    int id;
    char username[50];
    char content[280];
    int likes;
    int dislikes;
};
// User defined function for registering an user
void registerUser() {

    FILE *fp = fopen("user.txt", "a");
    struct User user;

    // Take input from user to register. 

    printf("Register New User\nUsername: ");
    scanf("%s", user.username);
    printf("Password: ");
    scanf("%s", user.password);

    // Store user data in the file

    fprintf(fp, "%s %s\n", user.username, user.password);
    fclose(fp);

    printf(GREEN "User registered successfully!\n" RESET);
}

// User defined function for user login 
int loginUser(struct User *loggedInUser) {
    FILE *fp = fopen("user.txt", "r");
    char uname[50], pwd[50];

    // Take input for user login 

    printf("Log In\nUsername: ");
    scanf("%s", uname);
    printf("Password: ");
    scanf("%s", pwd);

    // Fetching data from the text file and compare with the user input
    while (fscanf(fp, "%s %s", loggedInUser->username, loggedInUser->password) != EOF) {
        if (strcmp(uname, loggedInUser->username) == 0 && strcmp(pwd, loggedInUser->password) == 0) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// User defined function for generating post id 

int generatePostId() {
    FILE *fp = fopen("posts.txt", "r");
    int maxId = 0;
    struct Post post;

    if (!fp) return 1;

    // Scan data from posts and generate post id

    while (fscanf(fp, "%d|%49[^|]|%279[^|]|%d|%d\n", &post.id, post.username, post.content, &post.likes, &post.dislikes) == 5) {
        if (post.id > maxId) maxId = post.id;
    }

    fclose(fp);
    return maxId + 1;
}

// User defined function for creating post 

void createPost(const char *username) {
    FILE *fp = fopen("posts.txt", "a");
    struct Post post;

    // Assign post if\d from generatePostId() function
    post.id = generatePostId();
    strcpy(post.username, username);

    // Taking input for post from user. 
    printf("Enter post content: ");
    getchar();
    // Taking post content from user using fgets
    fgets(post.content, sizeof(post.content), stdin);
    post.content[strcspn(post.content, "\n")] = '\0';

    // Initialize the like and dislike 
    post.likes = 0;
    post.dislikes = 0;

    // Store the post data in the file 
    fprintf(fp, "%d|%s|%s|%d|%d\n", post.id, post.username, post.content, post.likes, post.dislikes);
    fclose(fp);

    printf(GREEN "Post created successfully!\n" RESET);
}

// User defined function for like or dislike a post 

void likeOrDislikePost(int isLike, int postId) {
    FILE *fp = fopen("posts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Post post;

    // Initializing the search result to 0
    int found = 0;

    // Fetching post data
    while (fscanf(fp, "%d|%49[^|]|%279[^|]|%d|%d\n", &post.id, post.username, post.content, &post.likes, &post.dislikes) == 5) {
        // Increase post like and dislike based on choice 
        if (post.id == postId) {
            if (isLike)
                post.likes++;
            else
                post.dislikes++;
            found = 1;
        }
        // Updating post like and dislike in the post 
        fprintf(temp, "%d|%s|%s|%d|%d\n", post.id, post.username, post.content, post.likes, post.dislikes);
    }

    fclose(fp);
    fclose(temp);

    // Edit on the temp file then rename it and delete old one 
    remove("posts.txt");
    rename("temp.txt", "posts.txt");

    // Printing success or error message after like or dislike operarion 
    if (found)
        printf(GREEN "Post %sed successfully!\n" RESET, isLike ? "lik" : "dislik");
    else
        printf(RED "Post not found.\n" RESET);
}

// User defined function to view all posts

void viewAllPosts() {
    FILE *fp = fopen("posts.txt", "r");
    struct Post post;

    printf(BLUE "\n----- All Posts -----\n" RESET);

    // Reading post data from the text file 
    while (fscanf(fp, "%d|%49[^|]|%279[^|]|%d|%d\n", &post.id, post.username, post.content, &post.likes, &post.dislikes) == 5) {
        printf("#%d by %s\n%s\nLikes: %d | Dislikes: %d\n----------------------\n",
               post.id, post.username, post.content, post.likes, post.dislikes);
    }

    fclose(fp);

    // Adding new option for user to like or dislike a post by inputting the id. 
    int action, postId;
    printf(MAGENTA "Do you want to (1) Like or (2) Dislike a post? (Enter 0 to skip): " RESET);
    scanf("%d", &action);

    // Pass the choice to likeOrDislikePost() function. 
    if (action == 1 || action == 2) {
        printf("Enter post id: ");
        scanf("%d", &postId);
        likeOrDislikePost(action == 1, postId);
    }
}

// User defined function to view user profile 

void viewProfile(const char *username) {
    FILE *fp = fopen("posts.txt", "r");
    struct Post post;
    // Initializing the value of total post, likes and dislikes. 
    int totalPosts = 0, totalLikes = 0, totalDislikes = 0;


    // Showing the result of user profile 
    while (fscanf(fp, "%d|%49[^|]|%279[^|]|%d|%d\n", &post.id, post.username, post.content, &post.likes, &post.dislikes) == 5) {
        if (strcmp(post.username, username) == 0) {
            totalPosts++;
            totalLikes += post.likes;
            totalDislikes += post.dislikes;
        }
    }

    fclose(fp);

    printf(BLUE "\n----- %s's Profile -----\n" RESET, username);
    printf("Total Posts: %d\nTotal Likes: %d\nTotal Dislikes: %d\n", totalPosts, totalLikes, totalDislikes);
}

// User defined function to search post by username and post term

void searchPosts() {
    FILE *fp = fopen("posts.txt", "r");
    if (!fp) {
        printf(RED "Error: Unable to open posts file.\n" RESET);
        return;
    }

    struct Post post;
    char searchTerm[280];
    int found = 0;

    printf("\nEnter keyword or username to search: ");
    getchar(); 
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0'; 

    printf(BLUE "\n--- Search Results ---\n" RESET);

    // Searching from post using strstr() function and show result.  

    while (fscanf(fp, "%d|%49[^|]|%279[^|]|%d|%d\n", &post.id, post.username, post.content, &post.likes, &post.dislikes) == 5) {
        if (strstr(post.content, searchTerm) || strcmp(post.username, searchTerm) == 0) {
            printf(MAGENTA "#%d by %s\n" RESET "%s\nLikes: %d | Dislikes: %d\n----------------------\n",
                post.id, post.username, post.content, post.likes, post.dislikes);
            found = 1;
        }
    }

    if (!found) {
        printf(RED "No matching posts found.\n" RESET);
    }

    fclose(fp);
}

int main() {
    int choice;
    struct User currentUser;
    int isLoggedIn = 0;

    while (1) {
        // For not logged in phase
        if (!isLoggedIn) {
            printf(BLUE "\nWelcome to Microthreads!\n" RESET);
            printf(MAGENTA "1. Register\n2. Login\n3. Exit\nChoice: " RESET);
            scanf("%d", &choice);

            // choice 1 for register, 2 for login and 3 for exit.

            if (choice == 1) {
                registerUser();
            } else if (choice == 2) {
                if (loginUser(&currentUser)) {
                    printf(GREEN "Login Successful! Welcome, %s\n" RESET, currentUser.username);
                    isLoggedIn = 1;
                } else {
                    printf(RED "Login Failed! Try again.\n" RESET);
                }
            } else {
                break;
            }
            // For logged in phase 
        } else {
            printf(MAGENTA "\n1. Create Post\n2. View All Posts\n3. View My Profile\n4. Search Posts\n5. Logout\nChoice: " RESET);
            scanf("%d", &choice);

            if (choice == 1) {
                createPost(currentUser.username);
            } else if (choice == 2) {
                viewAllPosts();
            } else if (choice == 3) {
                viewProfile(currentUser.username);
            } else if (choice == 4) {
                searchPosts();
            }else if(choice == 5){
                // make the value to 0 for logout
                isLoggedIn = 0;
                printf(GREEN "Logged out successfully.\n" RESET);
            } else {
                printf(RED "Invalid Option.\n" RESET);
            }
        }
    }

    return 0;
}

#include <gtk/gtk.h>
#include <fcntl.h>
#include <unistd.h>


  GtkWidget *window;

  GtkWidget *entry1;
  GtkWidget *entry2;

  GtkWidget *c_button1;
  GtkWidget *c_button2;

  GtkWidget *combo;

  GtkWidget *textview;

  GtkWidget *fixed;

//========================================================= add  BUTTONS Handlers===============================================

void add1_clicked(GtkWidget *widget, gpointer data) {
    
  FILE *fd;
  int fd1;
  int len;
  char temp[100],nu[4];
  GtkWidget *label;
 
  if(0==(int)gtk_entry_get_text_length(GTK_ENTRY(entry1))){
	show_error();
	goto NODATA;
	}


  fd = fopen("/etc/lsm/app_security.cfg","a+");


  fseek(fd,0,SEEK_END);	
  fwrite((char *)gtk_entry_get_text(GTK_ENTRY(entry1)),(int)gtk_entry_get_text_length(GTK_ENTRY(entry1)),1,fd);
  fseek(fd,0,SEEK_END);
  fwrite("\0\n",2,1,fd);
			

  fd1 = open("log.txt",O_CREAT|O_RDWR,0666);
  sprintf(temp,"APP list : %s\n",(char *)gtk_entry_get_text(GTK_ENTRY(entry1)));
  lseek(fd1,0,SEEK_END);	
  write(fd1,temp,strlen(temp));

//  fseek(fd1,0,SEEK_END);   (int)gtk_entry_get_text_length(GTK_ENTRY(entry1))+12
//  fwrite("\0\n",2,1,fd1);

NODATA:
  gtk_entry_set_text(GTK_ENTRY(entry1),"");
  g_print("clicked\n");
 
}
//===========
void add2_clicked(GtkWidget *widget, gpointer data) {

 FILE *fd;
 int fd1;
  int len;
  char temp[100],nu[4];
  GtkWidget *label;
 
  if(0==(int)gtk_entry_get_text_length(GTK_ENTRY(entry2))){
	show_error();
	goto NODATA;
	}


  fd = fopen("/etc/lsm/file_security.cfg","a+");


  fseek(fd,0,SEEK_END);	
  fwrite((char *)gtk_entry_get_text(GTK_ENTRY(entry2)),(int)gtk_entry_get_text_length(GTK_ENTRY(entry2)),1,fd);
  fseek(fd,0,SEEK_END);
  fwrite("\n",1,1,fd);
			

  fd1 = open("log.txt",O_CREAT|O_RDWR);
  sprintf(temp,"FILE list : %s\n",(char *)gtk_entry_get_text(GTK_ENTRY(entry2)));
  lseek(fd1,0,SEEK_END);	
  write(fd1,temp,strlen(temp));
//  fseek(fd1,0,SEEK_END);
//  fwrite("\0\n",2,1,fd1);

NODATA:
  gtk_entry_set_text(GTK_ENTRY(entry2),"");
  g_print("clicked\n");
}

//========================================================= choose  BUTTONS Handlers===============================================
void choose1_clicked(GtkWidget *widget, gpointer data) {
 
     gtk_entry_set_text(GTK_ENTRY(entry1),gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (c_button1)));
}


void choose2_clicked(GtkWidget *widget, gpointer data) {
 
     gtk_entry_set_text(GTK_ENTRY(entry2),gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (c_button2)));
}


//========================================================= view  BUTTON Handlers===============================================
void v_a_clicked(GtkWidget *widget, gpointer data) {

  FILE *fd;
  size_t len;
  ssize_t res;
  char *buf;
  GtkTextBuffer *buff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));

  fd = fopen("app_security.cfg","r");

  while ((res = getline(&buf, &len, fd)) != -1) {
     if(buf[0]!='#')
        printf("\n%s", buf);
//	  gtk_text_buffer_set_text (buff, buf, -1);
	}

//   free(buf);
}

//===========

void v_f_clicked(GtkWidget *widget, gpointer data) {

  FILE *fd;
  int i=148;
  size_t len;
  ssize_t res;
  char *buf;
  GtkTextBuffer *buff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));

  fd = fopen("app_security.cfg","r");

  while ((res = getline(&buf, &len, fd)) != -1) {
     if(buf[0]!='#')
        printf("\n%s", buf);
	  gtk_text_buffer_set_text (buff, buf, -1);
	}

   free(buf);
}

//========================================================= generate  BUTTON Handler===============================================

void g_clicked(GtkWidget *widget, gpointer data) {

  int fd;
  char buf[100];

  fd = open("add_app.txt",O_CREAT|O_RDWR,S_IRWXU);
  perror("Status:");
  
  read(fd,buf,20);		
  gtk_text_view_set_buffer(textview,(GtkTextBuffer*)buf);
  
  g_print("clicked: %s\n",buf);
}

//===========================================================COmbo box handler======================================================



void combo_selected(GtkWidget *widget, gpointer window) {
   
  int fd;

  gchar *hash = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX(widget));
 

  remove("/etc/lsm/hash.cfg");

  fd = open("hash.cfg",O_CREAT|O_RDWR);

  write(fd,(char *)hash,strlen((char *)hash));

  g_free(hash);
}

//===========================================================LOg button handlers================================================

void view_log_clicked(GtkWidget *widget, gpointer data) {
  
  popen("gedit log.txt","w");
}


void clear_log_clicked(GtkWidget *widget, gpointer data) {

  int fd1;
  char *temp="==================LOG FILE=================\n\0"; 

  system("rm -rf log.txt");



  fd1 = open("log.txt",O_CREAT|O_RDWR,0666);
  write(fd1,temp,strlen(temp));
  close(fd1);
  
}


//===========================================================Help menu handler===================================================

void g_menu_help(GtkWidget *widget, gpointer window) {
   
  system("./help.sh");
}


//======================================================Dialogs==========================================================================



void show_info(GtkWidget *widget, gpointer window) {
    
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Download Completed");
  gtk_window_set_title(GTK_WINDOW(dialog), "Information");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_error(void) {
    
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"Add something...!");
  gtk_window_set_title(GTK_WINDOW(dialog), "Error");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_question(GtkWidget *widget, gpointer window) {
    
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Are you sure to quit?");
  gtk_window_set_title(GTK_WINDOW(dialog), "Question");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_warning(GtkWidget *widget, gpointer window) {
    
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_WARNING,GTK_BUTTONS_OK,"Unallowed operation");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}



//================================================================================================================================

int main(int argc, char *argv[]) {

  uid_t uid=getuid(), euid=geteuid();
    if (uid==0) {



  GtkWidget *button1;
  GtkWidget *button2;
 
  GtkWidget *v_a_button;
  GtkWidget *v_f_button;

  GtkWidget *s_g_button;

  GtkWidget *view_log_button;
  GtkWidget *clear_log_button;

  GtkWidget *label_combo;
  GtkWidget *label_app;
  GtkWidget *label_data;
  GtkWidget *label_console;

  GtkWidget *menubar;
  GtkWidget *fileMenu;
  GtkWidget *fileMi;
  GtkWidget *helpMi;

  GtkWidget *vbox;

 



  /*Window Initilization*/
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "User Utility");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_widget_show(window);

  /* Layout set to fixed*/
  fixed = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), fixed);


  /*  Buttons Initilization*/
  button1 = gtk_button_new_with_label("Add");
  gtk_fixed_put(GTK_FIXED(fixed), button1, 350, 40);
  gtk_widget_set_size_request(button1, 80, 30);
  g_signal_connect(G_OBJECT(button1), "clicked",G_CALLBACK(add1_clicked), NULL);


  button2 = gtk_button_new_with_label("Add");
  gtk_fixed_put(GTK_FIXED(fixed), button2, 350, 110);
  gtk_widget_set_size_request(button2, 80, 30);
  g_signal_connect(G_OBJECT(button2), "clicked",G_CALLBACK(add2_clicked), NULL);


  /*Entry boxs initilization*/
  entry1 = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(fixed), entry1, 10, 40);
  gtk_widget_set_size_request(entry1, 260, 30);

 
  entry2 = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(fixed), entry2, 10,110);
  gtk_widget_set_size_request(entry2, 260, 30);


  /*  File choosers Initilization */
  c_button1 = gtk_file_chooser_button_new ("Choose fast",GTK_FILE_CHOOSER_ACTION_OPEN);
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (c_button1),g_get_home_dir());
  gtk_fixed_put(GTK_FIXED(fixed), c_button1, 280, 40);
  gtk_widget_set_size_request(c_button1, 50, 30);
  g_signal_connect(G_OBJECT(c_button1), "file-set",G_CALLBACK(choose1_clicked), NULL);


  c_button2 = gtk_file_chooser_button_new ("Choose fast",GTK_FILE_CHOOSER_ACTION_OPEN);
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (c_button2),g_get_home_dir());
  gtk_fixed_put(GTK_FIXED(fixed), c_button2, 280, 110);
  gtk_widget_set_size_request(c_button2, 50, 30);
  g_signal_connect(G_OBJECT(c_button2), "file-set",G_CALLBACK(choose2_clicked), NULL);


  /* Label Initilization */
  label_app = gtk_label_new("Choose an application to add to the whitelist:");
  gtk_fixed_put(GTK_FIXED(fixed), label_app, 10, 25);

  label_data = gtk_label_new("Choose a file to protect:  ");
  gtk_fixed_put(GTK_FIXED(fixed), label_data, 10, 95);

  label_combo = gtk_label_new("Choose a hashing algo :");
  gtk_fixed_put(GTK_FIXED(fixed), label_combo, 10, 167);

  label_console = gtk_label_new("Console Window :  ");
  gtk_fixed_put(GTK_FIXED(fixed), label_console, 10, 245);


  /* Combobox Initilization */
  combo = gtk_combo_box_text_new();
  gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "SHA1\0\0");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "SHA2\0\0");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "SHA256\0");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "SHA512\0");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX(combo), "MD5\0\0");

  gtk_fixed_put(GTK_FIXED(fixed), combo, 160, 160);
  gtk_widget_set_size_request(combo, 260, 30);

  g_signal_connect(G_OBJECT(combo), "changed", G_CALLBACK(combo_selected), NULL);

  /* View Buttons */ 
  v_f_button = gtk_button_new_with_label("View Files");
  gtk_fixed_put(GTK_FIXED(fixed), v_f_button, 250, 200);
  gtk_widget_set_size_request(v_f_button, 150, 30);
  g_signal_connect(G_OBJECT(v_f_button), "clicked",G_CALLBACK(v_a_clicked), NULL);


  v_a_button = gtk_button_new_with_label("View Apps");
  gtk_fixed_put(GTK_FIXED(fixed), v_a_button, 10, 200);
  gtk_widget_set_size_request(v_a_button, 150, 30);
  g_signal_connect(G_OBJECT(v_a_button), "clicked",G_CALLBACK(v_a_clicked), NULL);



  /* Text view*/
  textview = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview),FALSE);
  gtk_fixed_put(GTK_FIXED(fixed), textview, 10, 265);
  gtk_widget_set_size_request(textview, 330, 250);




 /*Log Buttons */
  view_log_button = gtk_button_new_with_label("View Log");
  gtk_fixed_put(GTK_FIXED(fixed), view_log_button, 350, 270);
  gtk_widget_set_size_request(view_log_button, 60, 30);
  g_signal_connect(G_OBJECT(view_log_button), "clicked",G_CALLBACK(view_log_clicked), NULL);


  clear_log_button = gtk_button_new_with_label("Clear Log");
  gtk_fixed_put(GTK_FIXED(fixed), clear_log_button, 350, 320);
  gtk_widget_set_size_request(clear_log_button, 60, 30);
  g_signal_connect(G_OBJECT(clear_log_button), "clicked",G_CALLBACK(clear_log_clicked), NULL);


  /*Generate Button */
  s_g_button = gtk_button_new_with_label("Generate CFG");
  gtk_fixed_put(GTK_FIXED(fixed), s_g_button, 150, 530);
  gtk_widget_set_size_request(s_g_button, 150, 30);
  g_signal_connect(G_OBJECT(s_g_button), "clicked",G_CALLBACK(g_clicked), NULL);


  /*  Menu bar */
  menubar = gtk_menu_bar_new();
  fileMenu = gtk_menu_new();

  fileMi = gtk_menu_item_new_with_label("File");
  helpMi = gtk_menu_item_new_with_label("Help");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), helpMi);

  gtk_fixed_put(GTK_FIXED(fixed), menubar, 0, 0);

  g_signal_connect(G_OBJECT(helpMi), "activate",G_CALLBACK(g_menu_help), NULL);


  gtk_widget_show_all(window);

  /* Close button signal */
  g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL);

  gtk_main();
}
else{
	printf("You are not a sudo user\nRun this application using sudo \n");
}

  return 0;
}



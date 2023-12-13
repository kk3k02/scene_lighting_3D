/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj� osi 
//  uk�adu wsp�rz�dnych dla rzutowania perspektywicznego

/*************************************************************************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

using namespace std;

typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
// inicjalizacja po�o�enia obserwatora

static GLfloat theta = 0.0;
// k�t obrotu obiektu wok� osi x

static GLfloat gamma = 0.0;
// k�t obrotu obiektu wok� osi y

static GLfloat pix2angle;
static GLfloat piy2angle;
// przelicznik pikseli na stopnie

static GLint status_left = 0;
static GLint status_right = 0;
// stan klawiszy myszy
// 0 - nie naci�ni�to �adnego klawisza
// 1 - naci�ni�ty zosta� lewy klawisz

static int delta_x = 0;
// r�nica pomi�dzy pozycj� bie��c�
// i poprzedni� kursora myszy (x)

static int delta_y = 0;
// r�nica pomi�dzy pozycj� bie��c�
// i poprzedni� kursora myszy (y)

static int delta_zoom = 0;
// r�nica pomi�dzy pozycj� bie��c�
// i poprzedni� zoom

static int x_pos_old = 0;
// poprzednia pozycja kursora myszy (x)

static int y_pos_old = 0;
// poprzednia pozycja kursora myszy (y)

static int zoom_pos_old = 0;
// poprzednia pozycja zoom


/*************************************************************************************/

// Funkcja rysuj�ca osie uk�adu wsp�?rz?dnych
void Axes(void)
{

    point3  x_min = { -5.0, 0.0, 0.0 };
    point3  x_max = { 5.0, 0.0, 0.0 };
    // pocz?tek i koniec obrazu osi x

    point3  y_min = { 0.0, -5.0, 0.0 };
    point3  y_max = { 0.0,  5.0, 0.0 };
    // pocz?tek i koniec obrazu osi y

    point3  z_min = { 0.0, 0.0, -5.0 };
    point3  z_max = { 0.0, 0.0,  5.0 };
    //  pocz?tek i koniec obrazu osi y
    glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
    glBegin(GL_LINES); // rysowanie osi x
    glVertex3fv(x_min);
    glVertex3fv(x_max);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
    glBegin(GL_LINES);  // rysowanie osi y

    glVertex3fv(y_min);
    glVertex3fv(y_max);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
    glBegin(GL_LINES); // rysowanie osi z

    glVertex3fv(z_min);
    glVertex3fv(z_max);
    glEnd();

}

/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia warto�ci odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{


    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && btn != GLUT_RIGHT_BUTTON)
    {
        x_pos_old = x;
        y_pos_old = y;
        // przypisanie aktualnie odczytanej pozycji kursora
        // jako pozycji poprzedniej
        status_left = 1;
        // wci�ni�ty zosta� lewy klawisz myszy
    }
    else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && btn != GLUT_LEFT_BUTTON)
    {
        zoom_pos_old = x;
        // przypisanie aktualnie odczytanej pozycji kursora
        // jako pozycji poprzedniej
        status_right = 1;
        // wci�ni�ty zosta� prawy klawisz myszy
    }
    else
    {
        status_left = 0;
        status_right = 0;
    }
    // nie zosta� wci�ni�ty �aden klawisz
}

/*************************************************************************************/
// Funkcja "monitoruje" po�o�enie kursora myszy i ustawia warto�ci odpowiednich
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

    delta_x = x - x_pos_old;
    delta_y = y - y_pos_old;
    delta_zoom = x - x_pos_old;
    // obliczenie r�nicy po�o�enia kursora myszy

    x_pos_old = x;
    y_pos_old = y;
    zoom_pos_old = x;
    // podstawienie bie��cego po�o�enia jako poprzednie

    glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/

/*************************************************************************************/

// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana, gdy trzeba
// przerysowa� scen�)



void RenderScene(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszcz�cym

    glLoadIdentity();
    // Czyszczenie macierzy bie??cej

    gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // Zdefiniowanie po�o�enia obserwatora
    Axes();
    // Narysowanie osi przy pomocy funkcji zdefiniowanej powy�ej

    if (status_left == 1 && status_right == 0)                     // je�li lewy klawisz myszy wci�ni�ty
    {
        theta += delta_x * pix2angle;
        gamma += delta_y * piy2angle;
        // modyfikacja k�ta obrotu o kat proporcjonalny
        // do r�nicy po�o�e� kursora myszy
    }
    if (status_right == 1 && status_left == 0)                     // je�li prawy klawisz myszy wci�ni�ty
    {
        viewer[2] += delta_zoom * pix2angle;
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    // Ustawienie koloru rysowania na bia�y

    glRotatef(theta, 0.0, 1.0, 0.0);  //obr�t obiektu o nowy k�t (x)
    glRotatef(gamma, 1.0, 0.0, 0.0);  //obr�t obiektu o nowy k�t (y)

    glutSolidTeapot(3.0);
    // Narysowanie czajnika

    glFlush();
    // Przekazanie polece� rysuj�cych do wykonania

    glutSwapBuffers();
}
/*************************************************************************************/

// Funkcja ustalaj�ca stan renderowania



void MyInit(void)
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Kolor czyszcz�cy (wype�nienia okna) ustawiono na czarny


    /*************************************************************************************/

//  Definicja materia�u z jakiego zrobiony jest czajnik
//  i definicja �r�d�a �wiat�a

/*************************************************************************************/


/*************************************************************************************/
// Definicja materia�u z jakiego zrobiony jest czajnik

    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    // wsp�czynniki ka =[kar,kag,kab] dla �wiat�a otoczenia

    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    // wsp�czynniki kd =[kdr,kdg,kdb] �wiat�a rozproszonego

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    // wsp�czynniki ks =[ksr,ksg,ksb] dla �wiat�a odbitego               

    GLfloat mat_shininess = { 20.0 };
    // wsp�czynnik n opisuj�cy po�ysk powierzchni


/*************************************************************************************/
// Definicja �r�d�a �wiat�a


    GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
    // po�o�enie �r�d�a


    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    // sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
    // Ia = [Iar,Iag,Iab]

    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    // sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
    // odbicie dyfuzyjne Id = [Idr,Idg,Idb]

    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    // sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
    // odbicie kierunkowe Is = [Isr,Isg,Isb]

    GLfloat att_constant = { 1.0 };
    // sk�adowa sta�a ds dla modelu zmian o�wietlenia w funkcji
    // odleg�o�ci od �r�d�a

    GLfloat att_linear = { 0.05 };
    // sk�adowa liniowa dl dla modelu zmian o�wietlenia w funkcji
    // odleg�o�ci od �r�d�a

    GLfloat att_quadratic = { 0.001 };
    // sk�adowa kwadratowa dq dla modelu zmian o�wietlenia w funkcji
    // odleg�o�ci od �r�d�a

/*************************************************************************************/
// Ustawienie parametr�w materia�u i �r�d�a �wiat�a

/*************************************************************************************/
// Ustawienie patrametr�w materia�u


    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);


    /*************************************************************************************/
    // Ustawienie parametr�w �r�d�a

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);


    /*************************************************************************************/
    // Ustawienie opcji systemu o�wietlania sceny

    glShadeModel(GL_SMOOTH); // w�aczenie �agodnego cieniowania
    glEnable(GL_LIGHTING);   // w�aczenie systemu o�wietlenia sceny
    glEnable(GL_LIGHT0);     // w��czenie �r�d�a o numerze 0
    glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora

    /*************************************************************************************/
}

/*************************************************************************************/


// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s� 
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
    pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie (x)
    piy2angle = 360.0 / (float)vertical; // przeliczenie pikseli na stopnie (y)

    glMatrixMode(GL_PROJECTION);
    // Prze��czenie macierzy bie��cej na macierz projekcji

    glLoadIdentity();
    // Czyszcznie macierzy bie��cej

    gluPerspective(70, 1.0, 1.0, 30.0);
    // Ustawienie parametr�w dla rzutu perspektywicznego


    if (horizontal <= vertical)
        glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

    else
        glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
    // Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
    // relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

    glMatrixMode(GL_MODELVIEW);
    // Prze��czenie macierzy bie��cej na macierz widoku modelu  

    glLoadIdentity();
    // Czyszczenie macierzy bie��cej

}

/*************************************************************************************/

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli



void main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(300, 300);

    glutCreateWindow("Rzutowanie perspektywiczne");

    glutDisplayFunc(RenderScene);
    // Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
    // (callback function).  B�dzie ona wywo�ywana za ka�dym razem
    // gdy zajdzie potrzeba przerysowania okna


    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
    // za zmiany rozmiaru okna                       


    glutMouseFunc(Mouse);
    // Ustala funkcj� zwrotn� odpowiedzialn� za badanie stanu myszy

    glutMotionFunc(Motion);
    // Ustala funkcj� zwrotn� odpowiedzialn� za badanie ruchu myszy

    glEnable(GL_DEPTH_TEST);
    // W��czenie mechanizmu usuwania niewidocznych element�w sceny

    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/
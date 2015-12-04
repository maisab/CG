#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <windows.h>
#include <gl/freeglut.h>
#include "glm-0.3.2/glm/glm.h"
#include "glm-0.3.2/glm/glmint.h"
#include "bibutil.h"

#define PTS 10
#define LIMITES 200
#define LIMITEV 100
#define LIMITEA 20
#define LIMITEM 2000
#define RELOGIO 0.001
#define FORCAG 100

//CONST_GRAV 6,673 * exp10(-11) m^3 kg^-1 s^-2
float alpha=50;
GLfloat obsX, obsY, obsZ, obsX_ini, obsY_ini, obsZ_ini;
int x_ini, y_ini, bot;

struct ponto {
    double x, y, z;
    double xtmp, ytmp, ztmp;
    double vx, vy, vz;
    double vxtmp, vytmp, vztmp;
    double ax, ay, az;
    double fx, fy, fz;
    double massa;
};

struct meshPts {
    float *x, *y, *z;
    int numPts;
};

struct ponto p[PTS];
struct meshPts mp;
float cor1[PTS], cor2[PTS], cor3[PTS];
FILE *fp;
GLMmodel *model;
GLMmodel *model2;

OBJ *objetoSofa;
OBJ *objetoChair;
OBJ *objetoChamine;
OBJ *objetoPiano;
OBJ *objetoLustre;
GLfloat angle, fAspect;
//float angle = 0;

// Função callback chamada para fazer o desenho

void Desenha(void) {
    //int i;
//glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);


    // Limpa a janela de visualização com a cor de fundo especificada
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



// FAZER ISSO;
//PARA CADA OBJETO;
//DEPOIS DEFINIMOS A  LUZ
    glColor3f(0.7f, 0.5f, 0.22f);
    //glmDraw(model2,GLM_SMOOTH|GLM_TEXTURE|GLM_ MATERIAL);
    glPushMatrix();
    //glTranslatef(-10.0, -1.0, 0.0);//sofa
    glRotatef(90.0, 0,1, 0);
    glScalef(0.6f, 0.6f, 0.6f);
    glTranslatef(-14.0, 1, 3.4);

    //glTranslatef(0.0, -2.832, 0.0);// tapete;
    DesenhaObjeto(objetoSofa);
    glPopMatrix();



    glColor3f(0.7f, 0.5f, 0.22f);
    //glmDraw(model2,GLM_SMOOTH|GLM_TEXTURE|GLM_ MATERIAL);
    glPushMatrix();
    glTranslatef(-2, 1, 2);
    DesenhaObjeto(objetoChamine);
    glPopMatrix();
    glPushMatrix(); // salvar as coordenadas correntes
    glPopMatrix();

/*---------------------Piano------------------------*/
    glColor3f(0.0f, 0.0f, 0.0f);
    //glmDraw(model2,GLM_SMOOTH|GLM_TEXTURE|GLM_ MATERIAL);
    glPushMatrix();
    /*glRotatef(50, 0 , 1 , 0);
    glTranslatef(-6, 1, 2);*/

    glRotatef(90.0, 0 , 1, 0);
    glRotatef(90.0, 0 , 1, 0);
    glRotatef(90.0, 0 , 1, 0);
    //glScalef(0.2f, 0.2f, 0.2f);
    glTranslatef(3.0, 1, 8);
    DesenhaObjeto(objetoPiano);
    glPopMatrix();
    glPushMatrix(); // salvar as coordenadas correntes
    glPopMatrix();


/*------------------------Lustre--------------------*/

    glColor3f(0.0f, 0.0f, 0.0f);
    //glmDraw(model2,GLM_SMOOTH|GLM_TEXTURE|GLM_ MATERIAL);
    glPushMatrix();
    /*glRotatef(50, 0 , 1 , 0);
    glTranslatef(-6, 1, 2);*/

    //glRotatef(90.0, 0 , 1, 0);
    //glRotatef(90.0, 0 , 1, 0);
    //glRotatef(90.0, 0 , 1, 0);
    //glScalef(0.6f, 0.6f, 0.6f);
       // glTranslatef(-20, 1, 40);
    //glTranslatef(-2, 1, 2);
    DesenhaObjeto(objetoLustre);
    glPopMatrix();
    glPushMatrix(); // salvar as coordenadas correntes
    glPopMatrix();


    //ve como remove essa cor do sofa;
    //glTranslatef(0.0, -1.8, 0.0);
    //HERE IS WHERE I DRAW MY OBJ

    glmDraw(model, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
    glPopMatrix();
    glutSwapBuffers();
}



// Inicializa parâmetros de rendering

void Inicializa(void) {
    GLfloat luzAmbiente[4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat luzDifusaSofa[4] = {0.7, 0.7, 0.7, 1.0}; // "cor"
    GLfloat luzEspecularSofa[4] = {0.3, 0.5, 0.5, 1.0}; // "brilho"

    //GLfloat luzDifusaChair[4] = {0.2, 0.6, 0.2, 1.0}; // "cor"
   // GLfloat luzEspecularChair[4] = {0.2, 0.4, 0.2, 1.0}; // "brilho"

     GLfloat luzDifusaChamine[4] = {0.2, 0.3, 0.7, 1.0}; // "cor/"
    GLfloat luzEspecularChamine[4] = {0.9, 0.2, 0.5, 1.0}; // "brilho"



    GLfloat luzDifusaPiano[4] = {0.2, 0.8, 0.2, 1.0}; // "cor"
    GLfloat luzEspecularPiano[4] = {0.3, 0.2, 0.5, 1.0}; // "brilho"
/*
        GLfloat luzDifusaSofa[4] = {0.7, 0.7, 0.7, 1.0}; // "cor"
    GLfloat luzEspecularSofa[4] = {0.7, 0.5, 0.5, 1.0}; // "brilho"

        GLfloat luzDifusaSofa[4] = {0.7, 0.7, 0.7, 1.0}; // "cor"
    GLfloat luzEspecularSofa[4] = {0.7, 0.5, 0.5, 1.0}; // "brilho"*/
    GLfloat posicaoLuz[4] = {700.0, 700.0, 700.0, 1.0};

    // Capacidade de brilho do material
    GLfloat especularidade[4] = {0.7, 0.5, 0.2, 0.0};
    GLint especMaterial = 70;
    GLfloat especularidadeChamine[4] = {0.9, 0.1, 0.2, 0.0};
    GLint especMaterialChamine = 20;

    GLfloat especularidadePiano[4] = {0.9, 0.1, 0.2, 0.0};
    GLint especMaterialPiano = 15;


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//cor meio laranja

    // Habilita o modelo de colorização de Gouraud
    glShadeModel(GL_SMOOTH);

    // Define a refletância do material
    glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
    // Define a concentração do brilho
    glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

    // Define os parâmetros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glEnable(GL_LIGHTING);
    // Habilita a luz de número 0
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
    //glEnable(GL_LIGHT2);



    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusaSofa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecularSofa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusaChamine);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecularChamine);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusaPiano);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecularPiano);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

    // Habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    //Habilita o uso de iluminação

    // Habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);


    angle = 60;

    // Carrega o objeto 3D
    objetoSofa = CarregaObjeto("couchPoofyPillows.obj", true);
    printf("Objeto carregado!");

    // E calcula o vetor normal em cada face
    if (objetoSofa->normais) {
        // Se já existirem normais no arquivo, apaga elas
        free(objetoSofa->normais);
        objetoSofa->normais_por_vertice = false;
    }
    CalculaNormaisPorFace(objetoSofa);


    /* objetoChair = CarregaObjeto("singleChair.obj", true);
    printf("Objeto carregado!");

    // E calcula o vetor normal em cada face
    if (objetoChair->normais) {
        // Se já existirem normais no arquivo, apaga elas
        free(objetoChair->normais);
        objetoChair->normais_por_vertice = false;
    }
    CalculaNormaisPorFace(objetoChair);*/



    glMaterialfv(GL_FRONT, GL_SPECULAR, especularidadeChamine);
    // Define a concentração do brilho
    glMateriali(GL_FRONT, GL_SHININESS, especMaterialChamine);


     objetoChamine = CarregaObjeto("cheminee.obj", true);
    printf("Objeto chamine carregado!");

    // E calcula o vetor normal em cada face
    if (objetoChamine->normais) {
        // Se já existirem normais no arquivo, apaga elas
        free(objetoChamine->normais);
        objetoChamine->normais_por_vertice = false;
    }
    CalculaNormaisPorFace(objetoChamine);


/*-----Piano---*/

    glMaterialfv(GL_FRONT, GL_SPECULAR, especularidadePiano);
    // Define a concentração do brilho
    glMateriali(GL_FRONT, GL_SHININESS, especMaterialPiano);
    objetoPiano = CarregaObjeto("pianoDroit.obj", true);
    printf("Objeto piano carregado!");

    // E calcula o vetor normal em cada face
    if (objetoPiano->normais) {
        // Se já existirem normais no arquivo, apaga elas
        free(objetoPiano->normais);
        objetoPiano->normais_por_vertice = false;
    }
    CalculaNormaisPorFace(objetoPiano);


    /*Objeto lustre*/


    objetoLustre = CarregaObjeto("chandelier-lamp.obj", true);
    printf("Objeto lustre carregado!");

    // E calcula o vetor normal em cada face
    if (objetoLustre->normais) {
        // Se já existirem normais no arquivo, apaga elas
        free(objetoLustre->normais);
        objetoLustre->normais_por_vertice = false;
    }
    CalculaNormaisPorFace(objetoLustre);

}

// Função usada para especificar a posição do observador virtual

void PosicionaObservador(void) {
    obsX = obsY = 0;
    obsZ = 8;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); //carrega a matriz de identidade
    gluLookAt(0, 1.8, 10    , //posição da câmera
            0.0, 1.8, 0.0, //para onde a câmera aponta
            0.0, 1.0, 0.0); //vetor view-up//
    /*
    gluLookAt(0, 1.8, 10    , //posição da câmera
              0.0, 1.8, 0.0, //para onde a câmera aponta
              0.0, 1.0, 0.0); //vetor view-up//
    */
}

// Função usada para especificar o volume de visualização

void EspecificaParametrosVisualizacao(void) {

    // ativa a remocao das superficies traseiras;
    //depth_buffer(x,y) = 1.0



    // Especifica sistema de coordenadas de projeção
    glMatrixMode(GL_PROJECTION);
    // Inicializa sistema de coordenadas de projeção
    glLoadIdentity();

    // Especifica a projeção perspectiva
    gluPerspective(angle, fAspect, 0.1, 1000.0);


    PosicionaObservador();

}



// Função callback chamada para gerenciar eventos de teclas normais (ESC)
void Teclas (unsigned char tecla, int x, int y)
{
	if(tecla==27) // ESC ?
	{
		// Libera memória e finaliza programa
		LiberaObjeto(objetoSofa);
		exit(0);
	}
	if(tecla=='m')
	{
		if(glIsEnabled(GL_LIGHTING))
			glDisable(GL_LIGHTING);
		else
            glEnable(GL_LIGHTING);
	}
	glutPostRedisplay();
}

// Função callback para tratar eventos de teclas especiais
void TeclasEspeciais (int tecla, int x, int y)
{
	switch (tecla)
	{
		case GLUT_KEY_HOME:	if(angle>=10)  angle -=5;
							break;
		case GLUT_KEY_END:	if(angle<=150) angle +=5;
							break;
	}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

// Função callback para eventos de botões do mouse
/*void GerenciaMouse(int button, int state, int x, int y)
{
	if(state==GLUT_DOWN)
	{
		// Salva os parâmetros atuais
		x_ini = x;
		y_ini = y;
		obsX_ini = obsX;
		obsY_ini = obsY;
		obsZ_ini = obsZ;
		rotX_ini = rotX;
		rotY_ini = rotY;
		bot = button;
	}
	else bot = -1;
}*/

// Função callback para eventos de movimento do mouse
#define SENS_ROT	5.0
#define SENS_OBS	15.0
#define SENS_TRANSL	20.0
/*void GerenciaMovim(int x, int y)
{
	// Botão esquerdo
	if(bot==GLUT_LEFT_BUTTON)
	{
		// Calcula diferenças
		int deltax = x_ini - x;
		int deltay = y_ini - y;
		// E modifica ângulos
		rotY = rotY_ini - deltax/SENS_ROT;
		rotX = rotX_ini - deltay/SENS_ROT;
	}
	// Botão direito (zoom-in e zoom-out)
	else if(bot==GLUT_RIGHT_BUTTON){
		// Calcula diferença
		int deltaz = y_ini - y;
		// E modifica distância do observador
		obsZ = obsZ_ini + deltaz/SENS_OBS;
	}
	// Botão do meio
	else if(bot==GLUT_MIDDLE_BUTTON)
	{
		// Calcula diferenças
		int deltax = x_ini - x;
		int deltay = y_ini - y;
		// E modifica posições
		obsX = obsX_ini + deltax/SENS_TRANSL;
		obsY = obsY_ini - deltay/SENS_TRANSL;
	}
	PosicionaObservador();
	glutPostRedisplay();
}

*/
// Função callback chamada quando o tamanho da janela é alterado

void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
    // Para previnir uma divisão por zero
    if (h == 0) h = 1;

    // Especifica o tamanho da viewport
    glViewport(0, 0, w, h);

    // Calcula a correção de aspecto
    fAspect = (GLfloat) w / (GLfloat) h;

    EspecificaParametrosVisualizacao();
}

// Programa Principal

int main(int argc, char *argv[]) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // Especifica a posição inicial da janela GLUT
    glutInitWindowPosition(5, 5);
    glutInitWindowSize(1000, 800);
    glutCreateWindow("House");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    // Registra a função callback para tratamento das teclas normais
	//glutKeyboardFunc (Teclas);

	// Registra a função callback para tratamento das teclas especiais
	glutSpecialFunc (TeclasEspeciais);

	// Registra a função callback para eventos de botões do mouse
	//glutMouseFunc(GerenciaMouse);

	// Registra a função callback para eventos de movimento do mouse
	//glutMotionFunc(GerenciaMovim);
    glEnable(GL_DEPTH_TEST);
    model = glmReadOBJ("HouseOriginal2.obj");
    glmVertexNormals(model, 0.0, 0);
    Inicializa();
    glutMainLoop();

    return 0;
}

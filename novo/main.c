#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <windows.h>
#include <gl/glut.h>
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

struct ponto{
	double x,y,z;
	double xtmp,ytmp,ztmp;
	double vx,vy,vz;
	double vxtmp,vytmp,vztmp;
	double ax,ay,az;
	double fx,fy,fz;
	double massa;
};

struct meshPts{
    float *x,*y,*z;
    int numPts;
};

struct ponto p[PTS];
struct meshPts mp;
float cor1[PTS],cor2[PTS],cor3[PTS];
FILE *fp;
GLMmodel *model;
OBJ *objeto;
GLfloat angle, fAspect;
float angle=0;

// Função callback chamada para fazer o desenho
void Desenha(void)
{
     //int i;

     glMatrixMode(GL_MODELVIEW);
     //glLoadIdentity();

     // Limpa a janela de visualização com a cor de fundo especificada
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


     /*for(i=0;i<PTS;i++)
     {
         glColor3f(cor1[i], cor2[i], cor3[i]);

         glPushMatrix(); // salvar as coordenadas correntes

         glTranslatef(p[i].x,p[i].y,p[i].z);

         glutSolidSphere(10.0,20,20);

         glPopMatrix(); // volta para o sistema de coordenadas
     }
*/
     //Drawing the model

     glPushMatrix(); // salvar as coordenadas correntes

     angle += 1.0;
     glTranslatef(0.0,-1.8,500.0);
     glRotatef(angle,0.0,1.0,0.0);
     //HERE IS WHERE I DRAW MY OBJ
     glmDraw(model,GLM_SMOOTH|GLM_TEXTURE|GLM_MATERIAL);

     glPopMatrix();

     glutSwapBuffers();
}



// Inicializa parâmetros de rendering
void Inicializa (void)
{
    GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
	GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};	   // "cor"
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
	GLfloat posicaoLuz[4]={100.0, 900.0, 100.0, 1.0};

	// Capacidade de brilho do material
	GLfloat especularidade[4]={0.7,0.5,0.5,0.0};
	GLint especMaterial = 60;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Define a refletância do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);


    angle=60;
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
    // Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva
	gluPerspective(angle,fAspect,0.1,1000.0);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posição do observador e do alvo
	gluLookAt(0,0,500,0,0,0, 0,1,0);
}

// Função callback chamada pela GLUT a cada intervalo de tempo
// (a window não está sendo redimensionada ou movida)
void Timer(int value)
{
    int i,j;
    double ftmp;

    for(i=0;i<PTS;i++){

            p[i].fx = p[i].fy = p[i].fz = 0;

			for(j=0;j<PTS;j++){
				double dij;
				dij = sqrt((p[j].x - p[i].x)*(p[j].x - p[i].x) +
						(   p[j].y - p[i].y)*(p[j].y - p[i].y) +
						(p[j].z - p[i].z)*(p[j].z - p[i].z));
				if(i!=j){
					//ftmp = 6,673 * exp10(-11) * p[i].massa * p[j].massa/dij;
					ftmp = FORCAG * p[i].massa * p[j].massa/dij;
					p[i].fx += (p[j].x - p[i].x)*ftmp/dij;
					p[i].fy += (p[j].y - p[i].y)*ftmp/dij;
					p[i].fz += (p[j].z - p[i].z)*ftmp/dij;
				}
			}
			p[i].ax = p[i].fx/p[i].massa;
			p[i].ay = p[i].fy/p[i].massa;
			p[i].az = p[i].fz/p[i].massa;
	}

    for(i=0;i<PTS;i++){
			p[i].vxtmp = p[i].vx + p[i].ax*RELOGIO;
			p[i].vytmp = p[i].vy + p[i].ay*RELOGIO;
			p[i].vztmp = p[i].vz + p[i].az*RELOGIO;

			p[i].xtmp = p[i].x + p[i].vx*RELOGIO + p[i].ax*RELOGIO*RELOGIO/2;
			p[i].ytmp = p[i].y + p[i].vy*RELOGIO + p[i].ay*RELOGIO*RELOGIO/2;
			p[i].ztmp = p[i].z + p[i].vz*RELOGIO + p[i].az*RELOGIO*RELOGIO/2;
		}

		for(i=0;i<PTS;i++){
			p[i].vx = p[i].vxtmp;
			p[i].vy = p[i].vytmp;
			p[i].vz = p[i].vztmp;

			p[i].x = p[i].xtmp;
			p[i].y = p[i].ytmp;
			p[i].z = p[i].ztmp;
    }

    // Redesenha as esferas com as novas coordenadas
    glutPostRedisplay();
    glutTimerFunc(1,Timer, 1);
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;

	// Especifica o tamanho da viewport
	glViewport(0, 0, w, h);

	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;

	EspecificaParametrosVisualizacao();
}

// Programa Principal
int main( int argc, char *argv[])
{
    model=glmReadOBJ("objetos/HouseOriginal2.obj");
    glmVertexNormals(model,0.0,0);


    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000,800);
    glutCreateWindow("Interação Gravitacional");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutTimerFunc(1, Timer, 1);
    Inicializa();
    glutMainLoop();

    return 0;
}

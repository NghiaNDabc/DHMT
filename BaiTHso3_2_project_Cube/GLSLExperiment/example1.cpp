//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/


// remember to prototype
void generateGeometry( void );
void initGPUBuffers( void );
void shaderSetup( void );
void display( void );
void keyboard( unsigned char key, int x, int y );


typedef vec4 point4;
typedef vec4 color4;
using namespace std;

// Số các đỉnh của các tam giác
const int NumPoints = 36;

//point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
//color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
GLfloat l = -2.0, r = 2.0, bot = -2.0, top = 2.0, zNear = 4, zFar = 15;

point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program,model_loc, view_loc, projection_loc;
mat4 model, model_xetang = mat4();
const int soDinhHLP = 36;
const int soDinhHinhTru = 144;
const int soDinhCau = 360;
const int soDinhLa = 12;
const int soDinhParabol = 588;
int Index = 0;
point4 points[soDinhHLP + soDinhHinhTru + soDinhLa + soDinhCau + soDinhParabol]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[soDinhHLP + soDinhHinhTru + soDinhLa + soDinhCau + soDinhParabol]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normal[soDinhHLP + soDinhHinhTru + soDinhLa + soDinhCau + soDinhParabol];
point4 DinhHLP[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 MauHLP[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/
point4 DinhHtru[26]; GLfloat dr = M_PI / 180;
color4 MauHtru[26];
point4 DinhHinhCau[62];
color4 MauHinhCau[62];
//Dieu khien mo hinh
GLfloat quayAllX = 0, quayAllY = 0, quayAllZ = 0;
GLfloat quayBangXeTang = 0, quayNongSung = 0, quayKhungSung = 0, diChuyenXeTang = 0;
void initCube()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
	vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
	vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
	vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
	vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
	vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
	vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
	vertices[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colors[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colors[6] = color4(1.0, 1.0, 1.0, 1.0); // white
	vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
void quad(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}
void makeColorCube(void)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/

{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
void TaoDinhHtru()
{
	//hình trụ mặt trên
	int j = 0;
	for (int i = 0; i <= 11; i++) {
		DinhHtru[i] = point4(cos(j * dr), 1, -sin(j * dr), 1.0);
		j = j + 30;
	}
	DinhHtru[12] = point4(0, 1, 0, 1.0);
	//mặt dưới
	j = 0;
	for (int i = 13; i <= 24; i++) {
		DinhHtru[i] = point4(cos(j * dr), -1, -sin(j * dr), 1.0);
		j = j + 30;
	}
	DinhHtru[25] = point4(0, -1, 0, 1.0);
}
void MBHTru(int a, int b, int c, int d) {
	vec4 u = DinhHtru[b] - DinhHtru[a];
	vec4 v = DinhHtru[c] - DinhHtru[b];
	vec3 n = normalize(cross(u, v));
	normal[Index] = n; points[Index] = DinhHtru[a]; Index++;
	normal[Index] = n; points[Index] = DinhHtru[b]; Index++;
	normal[Index] = n; points[Index] = DinhHtru[c]; Index++;

	normal[Index] = n; points[Index] = DinhHtru[a]; Index++;
	normal[Index] = n; points[Index] = DinhHtru[c]; Index++;
	normal[Index] = n; points[Index] = DinhHtru[d]; Index++;
}
void MTHTru(int a[]) {
	vec4 u = DinhHtru[a[1]] - DinhHtru[a[7]];
	vec4 v = DinhHtru[a[4]] - DinhHtru[a[10]];
	vec3 n = normalize(cross(u, v));
	for (int i = 2; i <= 12; i++) {
		normal[Index] = n; points[Index] = DinhHtru[a[0]]; Index++;
		normal[Index] = n; points[Index] = DinhHtru[a[i - 1]]; Index++;
		normal[Index] = n; points[Index] = DinhHtru[a[i]]; Index++;
		if (i == 12) {
			normal[Index] = n; points[Index] = DinhHtru[a[0]]; Index++;
			normal[Index] = n; points[Index] = DinhHtru[a[12]]; Index++;
			normal[Index] = n; points[Index] = DinhHtru[a[1]]; Index++;
		}
	}

}
void HinhTru() {
	int a[] = { 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	int b[] = { 25, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
	//mặt tròn
	MTHTru(a);
	MTHTru(b);
	//thân hình trụ
	for (int i = 1; i <= 11; i++) {
		MBHTru(a[i], a[i + 1], b[i + 1], b[i]);
	}
	MBHTru(a[12], a[1], b[1], b[12]);
}
void VeHinhTru(mat4 instance, mat4 matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * instance);
	glDrawArrays(GL_TRIANGLES, soDinhHLP, soDinhHinhTru);    /*Vẽ các tam giác*/
}
void TaoDinhHinhCau() {
	float ratioSphere = 1;
	//hình tròn qua tâm
	int j = 0;
	for (int i = 0; i <= 11; i++) {
		DinhHinhCau[i] = point4(cos(j * dr), 0, sin(j * dr), 1.0);
		j = j + 30;
	}
	//hình tròn cách tâm sin 30 độ
	j = 0;
	ratioSphere = cos(30 * dr);
	for (int i = 12; i <= 23; i++) {
		DinhHinhCau[i] = point4(cos(j * dr) * ratioSphere, sin(30 * dr), sin(j * dr) * ratioSphere, 1.0);
		j = j + 30;
	}
	//hình tròn cách tâm sin 60 độ
	j = 0;
	ratioSphere = cos(60 * dr);
	for (int i = 24; i <= 35; i++) {
		DinhHinhCau[i] = point4(cos(j * dr) * ratioSphere, sin(60 * dr), sin(j * dr) * ratioSphere, 1.0);
		j = j + 30;
	}
	//hình tròn cách tâm - sin 30 độ
	j = 0;
	ratioSphere = cos(30 * dr);
	for (int i = 36; i <= 47; i++) {
		DinhHinhCau[i] = point4(cos(j * dr) * ratioSphere, -sin(30 * dr), sin(j * dr) * ratioSphere, 1.0);
		j = j + 30;
	}
	//hình tròn cách tâm - sin 60 độ
	j = 0;
	ratioSphere = cos(60 * dr);
	for (int i = 48; i <= 59; i++) {
		DinhHinhCau[i] = point4(cos(j * dr) * ratioSphere, -sin(60 * dr), sin(j * dr) * ratioSphere, 1.0);
		j = j + 30;
	}
	//đỉnh trên
	DinhHinhCau[60] = point4(0.0, sin(60 * dr), 0.0, 1.0);
	//đỉnh dưới
	DinhHinhCau[61] = point4(0.0, -sin(60 * dr), 0.0, 1.0);
}
void MBHinhCau(int a, int b, int c, int d) {
	vec4 u = DinhHinhCau[b] - DinhHinhCau[a];
	vec4 v = DinhHinhCau[c] - DinhHinhCau[b];
	vec3 n = normalize(cross(u, v));
	normal[Index] = n; points[Index] = DinhHinhCau[a]; Index++;
	normal[Index] = n; points[Index] = DinhHinhCau[b]; Index++;
	normal[Index] = n; points[Index] = DinhHinhCau[c]; Index++;

	normal[Index] = n; points[Index] = DinhHinhCau[a]; Index++;
	normal[Index] = n; points[Index] = DinhHinhCau[c]; Index++;
	normal[Index] = n; points[Index] = DinhHinhCau[d]; Index++;
}
void MTHinhCau(int a[], int pole) {
	vec4 u = DinhHinhCau[a[0]] - DinhHinhCau[a[6]];
	vec4 v = DinhHinhCau[a[4]] - DinhHinhCau[a[9]];
	vec3 n = normalize(cross(u, v));
	for (int i = 1; i <= 11; i++) {
		normal[Index] = n; points[Index] = DinhHinhCau[pole]; Index++;
		normal[Index] = n; points[Index] = DinhHinhCau[a[i - 1]]; Index++;
		normal[Index] = n; points[Index] = DinhHinhCau[a[i]]; Index++;
		if (i == 11) {
			normal[Index] = n; points[Index] = DinhHinhCau[pole]; Index++;
			normal[Index] = n; points[Index] = DinhHinhCau[a[11]]; Index++;
			normal[Index] = n; points[Index] = DinhHinhCau[a[0]]; Index++;
		}
	}
}
void HinhCau() {
	int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	int b[] = { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
	int c[] = { 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };
	int d[] = { 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 };
	int e[] = { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59 };
	//mặt cầu trên tâm
	for (int i = 0; i <= 10; i++) {
		MBHinhCau(a[i], a[i + 1], b[i + 1], b[i]);
	}
	MBHinhCau(a[11], a[0], b[0], b[11]);
	for (int i = 0; i <= 10; i++) {
		MBHinhCau(b[i], b[i + 1], c[i + 1], c[i]);
	}
	MBHinhCau(b[11], b[0], c[0], c[11]);
	//mặt cầu dưới tâm
	for (int i = 0; i <= 10; i++) {
		MBHinhCau(a[i], a[i + 1], d[i + 1], d[i]);
	}
	MBHinhCau(a[11], a[0], d[0], d[11]);
	for (int i = 0; i <= 10; i++) {
		MBHinhCau(d[i], d[i + 1], e[i + 1], e[i]);
	}
	MBHinhCau(d[11], d[0], e[0], e[11]);
	//mặt đỉnh trên
	MTHinhCau(c, 60);
	//mặt đỉnh dưới
	MTHinhCau(e, 61);
}
void VeHinhCau(mat4 instance, mat4 matrix) {
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * instance);
	glDrawArrays(GL_TRIANGLES, soDinhHLP + soDinhHinhTru, soDinhCau);    /*Vẽ các tam giác*/
}
void VeHinhHop(mat4 instance, mat4 matrix) {
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * instance);
	glDrawArrays(GL_TRIANGLES,0, NumPoints);    /*Vẽ các tam giác*/
}
void generateGeometry( void )
{
	//hinh lap phuong
	initCube();
	makeColorCube();
	//hinh tru
	TaoDinhHtru();
	HinhTru();
	//hinh cau
	TaoDinhHinhCau();
	HinhCau();
}


void initGPUBuffers( void )
{
	// Tạo một VAO - vertex array object
	GLuint vao;
    glGenVertexArrays( 1, &vao );     
    glBindVertexArray( vao );

    // Tạo và khởi tạo một buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), NULL, GL_STATIC_DRAW );

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors); 
	
}


void shaderSetup( void )
{
	// Nạp các shader và sử dụng chương trình shader
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );   // hàm InitShader khai báo trong Angel.h
    glUseProgram( program );

    // Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
    GLuint loc_vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( loc_vPosition );
    glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));
	model_loc = glGetUniformLocation(program, "model");
	projection_loc = glGetUniformLocation(program, "projection");
	view_loc = glGetUniformLocation(program, "view");
	glEnable(GL_DEPTH_TEST);
    glClearColor( 1.0, 1.0, 1.0, 1.0 );        /* Thiết lập màu trắng là màu xóa màn hình*/
}

void view_projection() {
	vec4 eye = vec4(3, 5, 10, 1);
	vec4 at = vec4(0, 0, 0, 1);
	vec4 up = vec4(0, 1, 0, 1);

	mat4 v = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, v);

	mat4 p = Ortho(l, r, bot, top, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, p);
}
void banhXeTang(GLfloat x, GLfloat y, GLfloat z) {
	mat4 instance = Translate(x, y, z) * RotateZ(quayBangXeTang) * RotateX(90) * Scale(0.1, 0.05, 0.1);
	VeHinhTru(instance, model_xetang);
}
void thanGiuaXeTang() {
	mat4 instance =  Scale(0.8, 0.2, 0.4);
	VeHinhHop(instance, model_xetang);
}
void thanXeTang() {
	banhXeTang(0.8 / 2 - 0.1 , 0, 0.4 / 2 - 0.05 / 2);
	banhXeTang(-(0.8 / 2 - 0.1 ), 0, 0.4 / 2 - 0.05 / 2);
	banhXeTang(0.8 / 2 - 0.1 , 0, -(0.4 / 2 - 0.05 / 2));
	banhXeTang(-(0.8 / 2 - 0.1 ), 0, -(0.4 / 2 - 0.05 / 2));
	mat4 instance = Translate(0,0.15+0.1,0)*Scale(0.8, 0.3, 0.4);
	VeHinhHop(instance, model_xetang);
}
void khungQuayXeTang() {
	mat4 instance = Scale(0.15, 0.1, 0.15);
	VeHinhTru(instance, model_xetang);
}
void nongSung() {
	mat4 instance = RotateZ(90) * Scale(0.05, 0.3, 0.05);
	VeHinhTru(instance, model_xetang);
}
void XeTang() {
	
	model_xetang = Translate(diChuyenXeTang, 0, 0)*Translate(0, 0.1, 0);
	thanXeTang();
	model_xetang = model_xetang *RotateY(quayKhungSung)* Translate(0,0.2+0.1 + 0.15 + 0.05,0);
	khungQuayXeTang();
	model_xetang = model_xetang * Translate(-0.25+0.15+0.2,0,0)*RotateZ(quayNongSung)*Translate(0.25, 0.05, 0);
	nongSung();
}

void display( void )
{
	
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );  
	view_projection();
	mat4 matrix = Translate(0, -0.33, 0) * RotateX(10) * Translate(0, 0.33, 0);
	mat4 instance = Translate(0, 0.2, 0) * Scale(0.2,0.2, 0.2);
	//VeHinhCau(instance,matrix);
	//model = RotateX(90)*Scale(0.3,0.3,0.3);
	//glUniformMatrix4fv(model_loc, 1, GL_TRUE, model);
	model = RotateX(quayAllX);
	XeTang();
	//glDrawArrays(GL_TRIANGLES, NumPoints, soDinhHinhTru);/*Vẽ các tam giác*/
	//glDrawArrays(GL_TRIANGLES, NumPoints, soDinhHinhTru);
	//glViewport(0, 0, 1000, 1000);
	//nongSung();
	//banhXeTang(0,0,0);
	//thanGiuaXeTang();
	glutSwapBuffers();									   
}


void keyboard( unsigned char key, int x, int y )
{
	// keyboard handler

    switch ( key ) {
	{
	
	case'q': quayNongSung  += 5;
		if (quayNongSung >= 45) quayNongSung = 45;
		glutPostRedisplay();
		break;
	case'Q': quayNongSung -= 5;
		if (quayNongSung <= -5) quayNongSung = -5;
		glutPostRedisplay();
		break;
	case'k': quayKhungSung += 5;
		glutPostRedisplay();
		break;
	case'K':
		quayKhungSung -= 5;
		glutPostRedisplay();
		break;
	case 'd': 
		quayBangXeTang -= 5;
		diChuyenXeTang += 0.01f;
		glutPostRedisplay();
		break;
	case 'D':
		quayBangXeTang += 5;
		diChuyenXeTang -= 0.01f;
		glutPostRedisplay();
		break;
	case 033:			// 033 is Escape key octal value
		exit(1);		// quit program
		break;
	}
	
    }
}


int main( int argc, char **argv )
{
	// main function: program starts here

    glutInit( &argc, argv );                       
    glutInitDisplayMode( GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize( 640, 640 );                 
	glutInitWindowPosition(100,150);               
    glutCreateWindow( "Drawing a Cube" );            

   
	glewInit();										

    generateGeometry( );                          
    initGPUBuffers( );							   
    shaderSetup( );                               

    glutDisplayFunc( display );                   
    glutKeyboardFunc( keyboard );                  

	glutMainLoop();
    return 0;
}

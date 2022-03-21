#include "GLOBJECT.h"


GLOBJECT::GLOBJECT() 
{
	this->color = nullptr;
	this->vertex = nullptr;
	this->normal = nullptr;
}
GLOBJECT::~GLOBJECT() {
	MEMORYCLEAR();
}
GLOBJECT::GLOBJECT(const GLOBJECT& to_copy) 
{
	MEMORYCLEAR();
	this->color =		(unsigned char*)malloc(to_copy.ver_size * sizeof(unsigned char));
	this->vertex =		(float*)malloc(to_copy.ver_size * sizeof(float));
	this->normal =		(float*)malloc(to_copy.norm_size * sizeof(float));
	this->uv =			(float*)malloc(to_copy.uv_size * sizeof(float));
	this->ind =			(unsigned int*)malloc(to_copy.ind_size * sizeof(unsigned int));
	this->image =		(unsigned char*)malloc(sizeof(unsigned) * 4 * to_copy.image_X * to_copy.image_Y);
	this->ver_size =	to_copy.ver_size; //data size
	this->norm_size =	to_copy.norm_size;
	this->uv_size =		to_copy.uv_size; //uv size
	this->ind_size =	to_copy.ind_size;
	this->image_X =		to_copy.image_X;
	this->image_Y =		to_copy.image_Y;
	std::memcpy(this->origin,	to_copy.origin,		sizeof(float) * 3); 
	std::memcpy(this->Rotation,	to_copy.Rotation,	sizeof(float) * 3); 
	std::memcpy(this->Scale,		to_copy.Scale,		sizeof(float) * 3); 
	std::memcpy(this->color,		to_copy.color,		sizeof(unsigned char) * this->ver_size);
	std::memcpy(this->vertex,	to_copy.vertex,		sizeof(float) * this->ver_size);
	std::memcpy(this->normal,	to_copy.normal,		sizeof(float) * this->norm_size); 
	std::memcpy(this->uv,		to_copy.uv,			sizeof(float) * this->uv_size); 
	std::memcpy(this->ind,		to_copy.ind,		sizeof(unsigned int) * this->ind_size);
	std::memcpy(this->image,		to_copy.image,		sizeof(char)  * 4 * this->image_X * this->image_Y);
	this->ridge_type = to_copy.ridge_type;
	this->rect =		to_copy.rect;
	this->sph =			to_copy.sph;
}
void GLOBJECT::MEMORYCLEAR() {
	if (this->color != nullptr)
		std::free(this->color);
	if (this->vertex != nullptr)
		std::free(this->vertex);
	if (this->normal != nullptr)
		std::free(this->normal);
	if (this->image != nullptr)
		std::free(this->image);
	if (this->uv != nullptr)
		std::free(this->uv);
	if (this->ind != nullptr)
		std::free(this->ind);
	this->color = nullptr;
	this->vertex = nullptr;
	this->normal = nullptr;
	this->image = nullptr;
	this->uv = nullptr;
	this->ind = nullptr;
}
void GLOBJECT::LOAD_STL(string filename) 
{
	MEMORYCLEAR();
	ESTL temp(filename, true);
	PUSHARRAY(NULL, temp.vertices, temp.normal, NULL, temp.indice,
		temp.num, temp.num, 0, temp.num);
}
void GLOBJECT::LOAD_OBJ(string filename)
{
	MEMORYCLEAR();
	EOBJ temp(filename);
	float* ver = &temp.vertices[0];
	float* norm = (temp.normals.size() > 0)?&temp.normals[0]:NULL;
	float* uv = (temp.text_coordinates.size() > 0) ?&temp.text_coordinates[0]:NULL;
	unsigned int* ind = &temp.ind[0];
	PUSHARRAY(NULL, ver, norm, uv, ind,
		temp.vertices.size(), temp.normals.size(), temp.text_coordinates.size(), temp.ind.size());
}
void GLOBJECT::LOAD_BMP(string filename, const char alpha) {
	EBMP temp(filename);
	this->image = (unsigned char*)malloc(sizeof(unsigned char) * temp.bi.biWidth * temp.bi.biHeight * 4);
	this->image_X = temp.bi.biWidth;
	this->image_Y = temp.bi.biHeight;
	for (unsigned int i = 0; i < temp.bi.biWidth * temp.bi.biHeight * 4; i+= 4)
	{
		this->image[i]		= (int)temp.brgb[i/4].r;
		this->image[i + 1]	= (int)temp.brgb[i/4].g;
		this->image[i + 2]	= (int)temp.brgb[i/4].b;
		this->image[i + 3]  =  alpha;
	}
}
void GLOBJECT::PUSHARRAY(const unsigned char* color, const float* vertex, const float* normal, const float* uv, const unsigned int* ind,
						const unsigned int ver_size, const unsigned norm_size,const unsigned int uv_size, const unsigned int ind_size) {
	MEMORYCLEAR();

	this->ver_size = ver_size;
	this->norm_size = norm_size;
	this->uv_size = uv_size;
	this->ind_size = ind_size;
	
	this->color = (unsigned char*)calloc(ver_size, sizeof(unsigned char));
	this->vertex = (float*)malloc(sizeof(float) * ver_size);
	this->normal = (float*)malloc(sizeof(float) * norm_size);
	
	this->uv = (float*)malloc(sizeof(float) * uv_size);
	this->ind = (unsigned int*)malloc(sizeof(unsigned int) * ind_size);

	if (color != NULL)
		std::memcpy(this->color, color, sizeof(unsigned char) * ver_size);
	else
		std::memset(this->color, 255, sizeof(unsigned char) * ver_size);
	if (vertex != NULL)
		std::memcpy(this->vertex, vertex, sizeof(float) * ver_size);
	if (normal != NULL)
		std::memcpy(this->normal, normal, sizeof(float) * norm_size);
	if (uv != NULL)
		std::memcpy(this->uv, uv, sizeof(float) * uv_size);
	if (ind != NULL)
		std::memcpy(this->ind, ind, sizeof(unsigned int) * ind_size);

}
void GLOBJECT::LOCATION(float x, float y, float z, float r1, float r2, float r3, float s1, float s2, float s3)
{
	this->origin[0] = x;
	this->origin[1] = y;
	this->origin[2] = z;
	this->Rotation[0] = r1;
	this->Rotation[1] = r2;
	this->Rotation[2] = r3;
	this->Scale[0] = s1;
	this->Scale[1] = s2;
	this->Scale[2] = s3;
}
void GLOBJECT::MOVE(float x, float y, float z, float r1, float r2, float r3, float s1, float s2, float s3)
{
	this->origin[0] += x;
	this->origin[1] += y;
	this->origin[2] += z;
	this->Rotation[0] += r1;
	this->Rotation[1] += r2;
	this->Rotation[2] += r3;
	this->Scale[0] += s1;
	this->Scale[1] += s2;
	this->Scale[2] += s3;
}
//DRAW ON OPENGL
void GLOBJECT::CREATE_IBO() {
	glewInit();
	//color
	glGenBuffers(1, &vbo_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	glBufferData(GL_ARRAY_BUFFER, this->ver_size * sizeof(GLubyte), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->ver_size * sizeof(GLubyte), this->color);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//vertices
	glGenBuffers(1, &vbo_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, this->ver_size * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->ver_size * sizeof(GLfloat), this->vertex);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (this->norm_size > 0)
	{
		//normal
		glGenBuffers(1, &vbo_normal);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
		glBufferData(GL_ARRAY_BUFFER, this->norm_size * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, this->norm_size * sizeof(GLfloat), this->normal);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (this->ind_size > 0)
	{
		//ind
		glGenBuffers(1, &vbo_ind);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ind);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->ind_size * sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, this->ind_size * sizeof(GLuint), this->ind);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	if (this->uv_size > 0)
	{
		//uv
		glGenBuffers(1, &vbo_uv);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
		glBufferData(GL_ARRAY_BUFFER, this->uv_size * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, this->uv_size * sizeof(GLfloat), this->uv);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (this->image_X > 0 && this->image_Y > 0)
	{
		//texture
		glGenTextures(1, &vbo_texture);
		glBindTexture(GL_TEXTURE_2D, vbo_texture);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, this->image_X, this->image_Y);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->image_X, this->image_Y, GL_RGBA, GL_UNSIGNED_BYTE, this->image);
		//texture option
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
void GLOBJECT::DRAW_IBO() {

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	glColorPointer(3, GL_UNSIGNED_BYTE, 0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glVertexPointer(3, GL_FLOAT, 0, 0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ind);

	if (this->norm_size > 0)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
		glNormalPointer(GL_FLOAT, 0, 0);
	}
	if (this->image_X > 0 && this->image_Y > 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, vbo_texture);
	}	
	if (this->uv_size > 0)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
	}
	//object draw
	glPushMatrix();

	glTranslatef(origin[0], origin[1], origin[2]);
	glRotatef(Rotation[0], 1, 0, 0);
	glRotatef(Rotation[1], 0, 0, 1);
	glRotatef(Rotation[2], 0, 1, 0);
	glScalef(Scale[0], Scale[1], Scale[2]);

	glDrawElements(GL_TRIANGLES, ind_size, GL_UNSIGNED_INT, 0);
	glPopMatrix();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	if (this->norm_size > 0)
	{
		glDisableClientState(GL_NORMAL_ARRAY);
		glDeleteBuffers(1, &vbo_normal);
	}
		
	if (this->image_X > 0 && this->image_Y > 0)
	{
		glDisable(GL_TEXTURE_2D);
		glDeleteTextures(1, &vbo_texture);
	}
	if (this->uv_size > 0)
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDeleteBuffers(1, &vbo_uv);
	}
	
	glDeleteBuffers(1, &vbo_vertex);
	glDeleteBuffers(1, &vbo_color);
	glDeleteBuffers(1, &vbo_ind);

}
//VBO DELETED AFTER DRAW
void GLOBJECT::DRAW() {

	glPushMatrix();

	glScalef(Scale[0], Scale[1], Scale[2]);
	glTranslatef(origin[0], origin[1], origin[2]);
	glRotatef(Rotation[0], 1, 0, 0);
	glRotatef(Rotation[1], 0, 0, 1);
	glRotatef(Rotation[2], 0, 1, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, color);
	glVertexPointer(3, GL_FLOAT, 0, vertex);
	/*
	if (this->TYPE == POLY)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, normal);
		glDrawArrays(GL_POLYGON, 0, size);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	else if (this->TYPE == TRI)
		glDrawArrays(GL_TRIANGLES, 0, size);
	else if (this->TYPE == QUAD)
		glDrawArrays(GL_QUADS, 0, size);
	else if (this->TYPE == LINE)
		glDrawArrays(GL_LINES, 0, size );
	else
		glDrawArrays(GL_POINTS, 0, size );
	*/
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glPopMatrix();
}
void GLOBJECT::pinfo() 
{
#pragma omp parallel for
	for (unsigned int i = 0; i < this->ver_size; i += 3)
		printf("COL [%d] [%d, %d, %d]\n", i / 3, this->color[i], this->color[i + 1], this->color[i + 2]);
#pragma omp parallel for
	for (unsigned int i = 0; i < this->ver_size; i += 3)
		printf("VEC [%d] [%f, %f, %f]\n",i/3, this->vertex[i], this->vertex[i + 1], this->vertex[i + 2]);
#pragma omp parallel for
	for (unsigned int i = 0; i < this->norm_size; i += 3)
		printf("NOR [%d] [%f, %f, %f]\n", i/3, this->normal[i], this->normal[i + 1], this->normal[i + 2]);
#pragma omp parallel for
	for (unsigned int i = 0; i < this->ind_size; i += 3)
		printf("indice [%d] [%d, %d, %d]\n", i/3, this->ind[i], this->ind[i + 1], this->ind[i + 2]);
}
//collider
void GLOBJECT::RIDGEBODY_SET(const char type)
{
	this->ridge_type = type;
	if (type == RECTANGLE)
		this->rect.body_set(this->vertex, this->ver_size);
	else if (type == SPHERE)
		this->sph.body_set(this->vertex, this->ver_size);
}
void GLOBJECT::RIDGEBODY_POS()
{
	if (this->ridge_type == RECTANGLE)
		rect.body_pos(origin, Scale, Rotation);
	else if (this->ridge_type == SPHERE)
		sph.body_pos(origin, Scale, Rotation);
}
void GLOBJECT::RIDGEBODY_DRAW()
{
	if (this->ridge_type == RECTANGLE)
	{
		glPointSize(10);
		glBegin(GL_POINTS);
		glColor4ub(255, 255, 0, 100);
		for (int i = 0; i < 8; i++)
			glVertex3f(rect.collision_pos[3 * i], rect.collision_pos[3 * i + 1], rect.collision_pos[3 * i + 2]);
		glVertex3f(rect.center_pos[0], rect.center_pos[1], rect.center_pos[2]);
		glEnd();
	}
	else if (this->ridge_type == SPHERE)
	{
		glPushMatrix();
		glColor4ub(255, 255, 0, 50);
		GLUquadric* quad;
		quad = gluNewQuadric();
		glTranslatef(this->sph.collision_pos[0], this->sph.collision_pos[1], this->sph.collision_pos[2]);
		gluSphere(quad, this->sph.new_radius, 40, 40);
		glPopMatrix();
	}
}
bool GLOBJECT::RIDGEBODY_COLLISION( GLOBJECT* trg)
{
	if (this->ridge_type == RECTANGLE && trg->ridge_type == RECTANGLE)
		return rect.OBB_detecting(&trg->rect);
	else if (this->ridge_type == SPHERE && trg->ridge_type == SPHERE)
		return sph.collision_two_sphere(&trg->sph);
}
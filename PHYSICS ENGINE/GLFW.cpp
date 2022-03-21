#include "GLFW.h"

	GLFW::GLFW(int w, int h) {
		this->w = w;
		this->h = h;
		this->window = nullptr;
		this->T[0] = 0, this->T[1] = 0, this->T[2] = 0;
		this->S[0] = 2, this->S[1] = 1;
		this->R = 0;
		this->xpos = 0; // mouse
		this->ypos = 0;
	}
	GLFW::~GLFW() {
		window_destroy();
	}
	void GLFW::run() {
		init_Window();	printf("init window\n");
		screen_init();  printf("init screen\n");
		screen_reshpae(); printf("reshape screen\n");
		MyHandler(); printf("handler added\n");
		mainLoop(); printf("loop end\n");
		window_destroy();
	}
	void GLFW::init_Window() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		glfwWindowHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glfwWindowHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glfwWindowHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glfwWindowHint(GLFW_SAMPLES, 16);

		this->window = glfwCreateWindow(w, h, "Open GL window", nullptr, nullptr);
		glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GLFW_TRUE);

	}

	void GLFW::screen_init() {
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSwapInterval(1);
		glEnable(GL_DEPTH_TEST);
	}
	void GLFW::screen_reshpae() {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glfwGetWindowSize(window, &w, &h);
		glViewport(0, 0, w , h );
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	void GLFW::screen_llight() {
		GLfloat lightPos[4] = { 0.5,0.5,0.5,1 };
		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		glEnable(GL_LIGHT0);
	}
	void GLFW::screen_clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		float fAspect = (float)w  / (float)h;
		gluPerspective(60.0, fAspect, 0.01, 500.0);
		glTranslatef(T[0], T[1], T[2]);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//1ST VIEW ROTATING
		if (mouse_bool)
		{
			double x, y;
			double px_max = (double)w / 4 * 3;
			double px_min = (double)w / 4;
			double py_max = (double)h / 4 * 3;
			double py_min = (double)h / 4;
			glfwGetCursorPos(window, &x, &y);
			(px_max < x) ? mycamera.CAMERA_1ST_ROTATION(LEFT, 0.1) :
				(px_min > x) ? mycamera.CAMERA_1ST_ROTATION(RIGHT, 0.1) : 0;
			(py_max < y) ? mycamera.CAMERA_1ST_ROTATION(UP, 0.1) :
				(py_min > y) ? mycamera.CAMERA_1ST_ROTATION(DOWN, 0.1) : 0;			
		}
		mycamera.LOOKAT();
		glRotatef(R, 0, 1, 0);		
	}

	void GLFW::mainLoop() {
		unsigned long temp = 0;
		while (!glfwWindowShouldClose(window)) {
			screen_clear();
			DRAW_COORDINATE();
			//myLight.LIGHT_ON();
			for (int i = 0; i < obj_list->size(); i++)
			{
				obj_list->at(i).CREATE_IBO();
				obj_list->at(i).DRAW_IBO();
				obj_list->at(i).RIDGEBODY_POS();
				obj_list->at(i).RIDGEBODY_DRAW();
			}
			glfwSwapBuffers(window);
			glfwPollEvents();
			//test collision
			int i = 1;
			for (int j = 0; j < obj_list->size(); j++)
			{
				if (j != i)
				{
					if (obj_list->at(i).RIDGEBODY_COLLISION(&obj_list->at(j)))
					{
						testv = -testv;
						break;
					}					
				}
			}
			testv -= 0.0005;
			obj_list->at(i).MOVE(testv, 0, testv, 0, 0, 0, 0, 0, 0);

		}
	}
	void GLFW::window_destroy() {
		glfwDestroyWindow(window);
		window = nullptr;
		glfwTerminate();
	}
	void GLFW::windowEvent(int width, int height)
	{
		screen_reshpae();
	}
	void GLFW::scrollEvent(double x, double y) 
	{
		if (y > 0)
			S[0] += 0.2 * S[0], S[1] += 0.2 * S[1];
		else if (y < 0)
			S[0] -= 0.2 * S[0], S[1] -= 0.2 * S[1];
	}
	void GLFW::mouseEvent(int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		{
			if(action == GLFW_PRESS && action != GLFW_REPEAT)
				glfwGetCursorPos(window, &xpos, &ypos);
			else if(action == GLFW_RELEASE)
			{
				double x, y;				
				glfwGetCursorPos(window, &x, &y);
				T[0] += (xpos < x) ? (x - xpos) /1000 : (x - xpos) /1000;
				T[1] -= (ypos < y) ? (y - ypos) /1000 : (y - ypos) /1000;
			}
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			if (action == GLFW_PRESS && action != GLFW_REPEAT)
				mouse_bool = true;
			else if (action == GLFW_RELEASE)
				mouse_bool = false;
	}
	void GLFW::keyEvent(int key, int scancode, int action, int mods) {

		if (key == GLFW_KEY_W)
			mycamera.CAMERA_1ST_MOVE(FORWARD, 0.05f);
		if (key == GLFW_KEY_S)
			mycamera.CAMERA_1ST_MOVE(BACKWARD, 0.05f);
		if (key == GLFW_KEY_A )
			mycamera.CAMERA_1ST_MOVE(LEFT, 0.05f);
		if (key == GLFW_KEY_D )
			mycamera.CAMERA_1ST_MOVE(RIGHT, 0.05f);
		if (key == GLFW_KEY_E )
			R -= 1;
		if (key == GLFW_KEY_Q )
			R += 1;
		if (key == GLFW_KEY_ENTER)
			T[0] = T[1] = T[2] = R = 0, S[0] = 2, S[1] = 1;
	}
	void GLFW::MyHandler()
	{
		glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
	}


	void DRAW_COORDINATE()
	{
		glLineWidth(2);
		glBegin(GL_LINES);
		glColor3ub(255, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(100, 0, 0);
		glColor3ub(0, 255, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 100, 0);
		glColor3ub(0, 0, 255);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 100);
		glEnd();
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		GLFW* handler = reinterpret_cast<GLFW*>(glfwGetWindowUserPointer(window));
		if (handler)
			handler->scrollEvent(xoffset, yoffset);
	}
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		GLFW* handler = reinterpret_cast<GLFW*>(glfwGetWindowUserPointer(window));
		if (handler)
			handler->keyEvent(key, scancode, action, mods);
	}
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		GLFW* handler = reinterpret_cast<GLFW*>(glfwGetWindowUserPointer(window));
		if (handler)
			handler->mouseEvent(button, action, mods);
	}
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		GLFW* handler = reinterpret_cast<GLFW*>(glfwGetWindowUserPointer(window));
		if (handler)
			handler->windowEvent(width, height);
	}


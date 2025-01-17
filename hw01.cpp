#include "cs345.cpp"


// NOTES /////////////////////////////////
// - in the structs, am using types that match those used in the STL format
// -- real32 is a 32 bit floating point number (float)
// -- u32 is an unsigned 32 bit integer (unsigned int on many platforms)
// - for Bambu Studio, positions are in mm
// -- 25.4 and MM(1.0) are equivalent
// - the y-axis points up
// - you can scale the part after the fact in Bambu Studio
//   (keep your code simple!)


// STRUCTS ///////////////////////////////

struct Triangle {
    real32 v1_x;
    real32 v1_y;
    real32 v1_z;
    real32 v2_x;
    real32 v2_y;
    real32 v2_z;
    real32 v3_x;
    real32 v3_y;
    real32 v3_z;
};

struct STL {
    u32 num_triangles;
    Triangle *triangles;
};


// FUNCTIONS /////////////////////////////
// you implement these ///////////////////

STL stl_load_ASCII(char *filename) {
    _SUPPRESS_COMPILER_WARNING_UNUSED_VARIABLE(filename);

    return {};
}

STL stl_load_binary(char *filename) {
    _SUPPRESS_COMPILER_WARNING_UNUSED_VARIABLE(filename);

    return {};
}

void stl_save_ASCII(STL *stl, char *filename) {
    FILE *file = fopen(filename, "w");
    ASSERT(file);

    fprintf(file, "solid\n");
    for (Triangle *triangle = stl->triangles; triangle < stl->triangles + stl->num_triangles; ++triangle) {
        fprintf(file, "   facet normal 0.0 0.0 0.0\n");
        fprintf(file, "   outer loop\n");
        fprintf(file, "      vertex %f %f %f\n", triangle->v1_x, triangle->v1_y, triangle->v1_z);
        fprintf(file, "      vertex %f %f %f\n", triangle->v2_x, triangle->v2_y, triangle->v2_z);
        fprintf(file, "      vertex %f %f %f\n", triangle->v3_x, triangle->v3_y, triangle->v3_z);
        fprintf(file, "   endloop\n");
        fprintf(file, "   endfacet\n");
    }
    fprintf(file, "endsolid\n");

    fclose(file);
}

void stl_save_binary(STL *stl, char *filename) {
    FILE *file = fopen(filename, "w");
    ASSERT(file);

    int num_bytes = 80 + 4 + 50 * stl->num_triangles;
    char *buffer = (char *) calloc(num_bytes, 1); {
        int offset = 80;
        memcpy(buffer + offset, &stl->num_triangles, 4);
        offset += 4;
        for (Triangle *triangle = stl->triangles; triangle < stl->triangles + stl->num_triangles; ++triangle) {
            offset += 12;
            memcpy(buffer + offset, triangle, 36);
            offset += 38;
        }
    }
    fwrite(buffer, 1, num_bytes, file);
    free(buffer);

    fclose(file);
}

// y is up
STL lithopane(char *filename) {
    _SUPPRESS_COMPILER_WARNING_UNUSED_VARIABLE(filename);

    real32 *y;
    int width;
    int height;
    {
        int number_of_channels;
        u8 *data = stbi_load(filename, &width, &height, &number_of_channels, 0);
        y = (real32 *) calloc(width * height, sizeof(real32));
        for (int j = 0; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                #define real real32 // TODO: FORNOW
                y[j * width + i] = LINEAR_REMAP(data[(j * width + i) * number_of_channels], 0.0, 255.0, 4.0, 0.4);
            }
        }
        stbi_image_free(data);
    }

    STL stl = {};
    stl.num_triangles = 4 * (width - 1) * (height - 1) + 4 * (width - 1) + 4 * (height - 1);
    stl.triangles = (Triangle *) calloc(stl.num_triangles, sizeof(Triangle));
    int k = 0;
    for (int j = 0; j < height - 1; ++j) {
        for (int i = 0; i < width - 1; ++i) {
            // // TOP
            //  3
            // 12
            stl.triangles[k].v1_x = i;
            stl.triangles[k].v1_y = j;
            stl.triangles[k].v1_z = y[(j) * width + (i)];
            stl.triangles[k].v2_x = i + 1;
            stl.triangles[k].v2_y = j;
            stl.triangles[k].v2_z = y[(j) * width + (i + 1)];
            stl.triangles[k].v3_x = i + 1;
            stl.triangles[k].v3_y = j + 1;
            stl.triangles[k].v3_z = y[(j + 1) * width + (i + 1)];
            ++k;
            // 32
            // 1
            stl.triangles[k].v1_x = i;
            stl.triangles[k].v1_y = j;
            stl.triangles[k].v1_z = y[(j) * width + i];
            stl.triangles[k].v2_x = i + 1;
            stl.triangles[k].v2_y = j + 1;
            stl.triangles[k].v2_z = y[(j + 1) * width + (i + 1)];
            stl.triangles[k].v3_x = i;
            stl.triangles[k].v3_y = j + 1;
            stl.triangles[k].v3_z = y[(j + 1) * width + (i)];
            ++k;

            // // BOTTOM
            //  2
            // 13
            stl.triangles[k].v1_x = i;
            stl.triangles[k].v1_y = j;
            stl.triangles[k].v1_z = 0;
            stl.triangles[k].v2_x = i + 1;
            stl.triangles[k].v2_y = j + 1;
            stl.triangles[k].v2_z = 0;
            stl.triangles[k].v3_x = i + 1;
            stl.triangles[k].v3_y = j;
            stl.triangles[k].v3_z = 0;
            ++k;
            // 23
            // 1
            stl.triangles[k].v1_x = i;
            stl.triangles[k].v1_y = j;
            stl.triangles[k].v1_z = 0;
            stl.triangles[k].v2_x = i;
            stl.triangles[k].v2_y = j + 1;
            stl.triangles[k].v2_z = 0;
            stl.triangles[k].v3_x = i + 1;
            stl.triangles[k].v3_y = j + 1;
            stl.triangles[k].v3_z = 0;
            ++k;
        }
    }

    // LEFT & RIGHT STRIPS
    for (int j = 0; j < height - 1; ++j) {
        stl.triangles[k].v1_x = 0;
        stl.triangles[k].v1_y = j;
        stl.triangles[k].v1_z = 0;
        stl.triangles[k].v2_x = 0;
        stl.triangles[k].v2_y = j;
        stl.triangles[k].v2_z = y[(j) * width];
        stl.triangles[k].v3_x = 0;
        stl.triangles[k].v3_y = j + 1;
        stl.triangles[k].v3_z = 0;
        ++k;
        stl.triangles[k].v1_x = 0;
        stl.triangles[k].v1_y = j;
        stl.triangles[k].v1_z = y[(j) * width];
        stl.triangles[k].v2_x = 0;
        stl.triangles[k].v2_y = j + 1;
        stl.triangles[k].v2_z = y[(j + 1) * width];
        stl.triangles[k].v3_x = 0;
        stl.triangles[k].v3_y = j + 1;
        stl.triangles[k].v3_z = 0;
        ++k;
        stl.triangles[k].v1_x = width - 1;
        stl.triangles[k].v1_y = j;
        stl.triangles[k].v1_z = 0;
        stl.triangles[k].v2_x = width - 1;
        stl.triangles[k].v2_y = j + 1;
        stl.triangles[k].v2_z = 0;
        stl.triangles[k].v3_x = width - 1;
        stl.triangles[k].v3_y = j;
        stl.triangles[k].v3_z = y[(j) * width + (width - 1)];
        ++k;
        stl.triangles[k].v1_x = width - 1;
        stl.triangles[k].v1_y = j;
        stl.triangles[k].v1_z = y[(j) * width + (width - 1)];
        stl.triangles[k].v2_x = width - 1;
        stl.triangles[k].v2_y = j + 1;
        stl.triangles[k].v2_z = 0;
        stl.triangles[k].v3_x = width - 1;
        stl.triangles[k].v3_y = j + 1;
        stl.triangles[k].v3_z = y[(j + 1) * width + (width - 1)];
        ++k;
    }

    // TOP & BOTTOM STRIPS
    for (int i = 0; i < width - 1; ++i) {
        stl.triangles[k].v1_x = i;
        stl.triangles[k].v1_y = 0;
        stl.triangles[k].v1_z = 0;
        stl.triangles[k].v2_x = i + 1;
        stl.triangles[k].v2_y = 0;
        stl.triangles[k].v2_z = 0;
        stl.triangles[k].v3_x = i;
        stl.triangles[k].v3_y = 0;
        stl.triangles[k].v3_z = y[i];
        ++k;
        stl.triangles[k].v1_x = i;
        stl.triangles[k].v1_y = 0;
        stl.triangles[k].v1_z = y[i];
        stl.triangles[k].v2_x = i + 1;
        stl.triangles[k].v2_y = 0;
        stl.triangles[k].v2_z = 0;
        stl.triangles[k].v3_x = i + 1;
        stl.triangles[k].v3_y = 0;
        stl.triangles[k].v3_z = y[i + 1];
        ++k;
        stl.triangles[k].v1_x = i;
        stl.triangles[k].v1_y = height - 1;
        stl.triangles[k].v1_z = 0;
        stl.triangles[k].v2_x = i;
        stl.triangles[k].v2_y = height - 1;
        stl.triangles[k].v2_z = y[(height - 1) * width + (i)];
        stl.triangles[k].v3_x = i + 1;
        stl.triangles[k].v3_y = height - 1;
        stl.triangles[k].v3_z = 0;
        ++k;
        stl.triangles[k].v1_x = i;
        stl.triangles[k].v1_y = height - 1;
        stl.triangles[k].v1_z = y[(height - 1) * width + (i)];
        stl.triangles[k].v2_x = i + 1;
        stl.triangles[k].v2_y = height - 1;
        stl.triangles[k].v2_z = y[(height - 1) * width + (i + 1)];
        stl.triangles[k].v3_x = i + 1;
        stl.triangles[k].v3_y = height - 1;
        stl.triangles[k].v3_z = 0;
        ++k;
    }
    return stl;
}

// FORWARD DELCARATIONS //////////////////
// i already implemented these for you ///

void stl_draw(Camera3D *camera, STL *stl);


// MAIN //////////////////////////////////

int main() {
    STL stl = {};

    // stl.num_triangles = 1;
    // stl.triangles = (Triangle *) calloc(stl.num_triangles, sizeof(Triangle));
    // stl.triangles[0] = { 0.0, 0.0, 0.0,   10.0, 0.0, 0.0,   0.0, 10.0, 0.0 };

    // stl.num_triangles = 12;
    // stl.triangles = (Triangle *) calloc(stl.num_triangles, sizeof(Triangle));
    // int k = 0;
    // stl.triangles[k++] = {  10.0f,  10.0f,  10.0f,  10.0f, -10.0f, -10.0f,  10.0f,  10.0f, -10.0f };
    // stl.triangles[k++] = {  10.0f,  10.0f,  10.0f,  10.0f, -10.0f,  10.0f,  10.0f, -10.0f, -10.0f };
    // stl.triangles[k++] = { -10.0f, -10.0f,  10.0f, -10.0f,  10.0f, -10.0f, -10.0f, -10.0f, -10.0f };
    // stl.triangles[k++] = { -10.0f, -10.0f,  10.0f, -10.0f,  10.0f,  10.0f, -10.0f,  10.0f, -10.0f };
    // stl.triangles[k++] = { -10.0f,  10.0f,  10.0f,  10.0f,  10.0f, -10.0f, -10.0f,  10.0f, -10.0f };
    // stl.triangles[k++] = { -10.0f,  10.0f,  10.0f,  10.0f,  10.0f,  10.0f,  10.0f,  10.0f, -10.0f };
    // stl.triangles[k++] = {  10.0f, -10.0f,  10.0f, -10.0f, -10.0f, -10.0f,  10.0f, -10.0f, -10.0f };
    // stl.triangles[k++] = {  10.0f, -10.0f,  10.0f, -10.0f, -10.0f,  10.0f, -10.0f, -10.0f, -10.0f };
    // stl.triangles[k++] = {  10.0f,  10.0f,  10.0f, -10.0f, -10.0f,  10.0f,  10.0f, -10.0f,  10.0f };
    // stl.triangles[k++] = {  10.0f,  10.0f,  10.0f, -10.0f,  10.0f,  10.0f, -10.0f, -10.0f,  10.0f };
    // stl.triangles[k++] = { -10.0f,  10.0f, -10.0f,  10.0f, -10.0f, -10.0f, -10.0f, -10.0f, -10.0f };
    // stl.triangles[k++] = { -10.0f,  10.0f, -10.0f,  10.0f,  10.0f, -10.0f,  10.0f, -10.0f, -10.0f };

    stl = lithopane("jeannie.png");

    Camera3D camera = { 100.0 };
    while (cow_begin_frame()) {
        camera_move(&camera);

        stl_draw(&camera, &stl);

        // TODO: text box

        // TODO: load_ASCII
        // TODO: load_binary

        if (gui_button("save ASCII")) {
            stl_save_ASCII(&stl, "out_ASCII.stl");
        }

        if (gui_button("save binary")) {
            stl_save_binary(&stl, "out_binary.stl");
        }
    }
}


// FORWARD DELCARED FUNCTIONS ////////////

void stl_draw(Camera3D *camera, STL *stl) {
    mat4 C_inv = camera_get_V(camera);
    eso_begin(camera_get_PV(camera), SOUP_OUTLINED_TRIANGLES);
    for (Triangle *triangle = stl->triangles; triangle < stl->triangles + stl->num_triangles; ++triangle) {
        vec3 v1 = { triangle->v1_x, triangle->v1_y, triangle->v1_z };
        vec3 v2 = { triangle->v2_x, triangle->v2_y, triangle->v2_z };
        vec3 v3 = { triangle->v3_x, triangle->v3_y, triangle->v3_z };
        vec3 n = transformNormal(C_inv, normalized(cross(v2 - v1, v3 - v1)));
        vec3 color = (n.z < 0) ? V3(1.0, 0.0, 0.0) : V3(0.5 + 0.5 * n.x, 0.5 + 0.5 * n.y, 1.0);
        eso_color(color);
        eso_vertex(v1);
        eso_vertex(v2);
        eso_vertex(v3);
    }
    eso_end();
}


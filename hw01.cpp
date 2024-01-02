#include "cs345.cpp"


// NOTES /////////////////////////////////
// - in the structs, am using types that match those used in the STL format
// -- real32 is a 32 bit floating point number (float)
// -- u32 is an unsigned 32 bit integer (unsigned int on many platforms)
// - for Bambu Studio, positions are in mm
// -- 25.4 and MM(1.0) are equivalent


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

// FORWARD DELCARATIONS //////////////////
// i already implemented these for you ///

void stl_draw(Camera3D *camera, STL *stl);


// MAIN //////////////////////////////////

int main() {
    STL stl = {};
    stl.num_triangles = 12;
    stl.triangles = (Triangle *) calloc(stl.num_triangles, sizeof(Triangle)); {
        int k = 0;
        stl.triangles[k++] = {  10.0f,  10.0f,  10.0f,  10.0f, -10.0f, -10.0f,  10.0f,  10.0f, -10.0f };
        stl.triangles[k++] = {  10.0f,  10.0f,  10.0f,  10.0f, -10.0f,  10.0f,  10.0f, -10.0f, -10.0f };
        stl.triangles[k++] = { -10.0f, -10.0f,  10.0f, -10.0f,  10.0f, -10.0f, -10.0f, -10.0f, -10.0f };
        stl.triangles[k++] = { -10.0f, -10.0f,  10.0f, -10.0f,  10.0f,  10.0f, -10.0f,  10.0f, -10.0f };
        stl.triangles[k++] = { -10.0f,  10.0f,  10.0f,  10.0f,  10.0f, -10.0f, -10.0f,  10.0f, -10.0f };
        stl.triangles[k++] = { -10.0f,  10.0f,  10.0f,  10.0f,  10.0f,  10.0f,  10.0f,  10.0f, -10.0f };
        stl.triangles[k++] = {  10.0f, -10.0f,  10.0f, -10.0f, -10.0f, -10.0f,  10.0f, -10.0f, -10.0f };
        stl.triangles[k++] = {  10.0f, -10.0f,  10.0f, -10.0f, -10.0f,  10.0f, -10.0f, -10.0f, -10.0f };
        stl.triangles[k++] = {  10.0f,  10.0f,  10.0f, -10.0f, -10.0f,  10.0f,  10.0f, -10.0f,  10.0f };
        stl.triangles[k++] = {  10.0f,  10.0f,  10.0f, -10.0f,  10.0f,  10.0f, -10.0f, -10.0f,  10.0f };
        stl.triangles[k++] = { -10.0f,  10.0f, -10.0f,  10.0f, -10.0f, -10.0f, -10.0f, -10.0f, -10.0f };
        stl.triangles[k++] = { -10.0f,  10.0f, -10.0f,  10.0f,  10.0f, -10.0f,  10.0f, -10.0f, -10.0f };
    }

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
    // NOTE: coloring should really be done in a shader
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


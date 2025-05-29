#ifndef retangulo_h
#define retangulo_h
    
    typedef void* RECTANGLE;


    RECTANGLE create_rectangle(int i, float x, float y, float w, float h, char* cb, char* cp);


    int get_idR(RECTANGLE r);
    void set_idR(RECTANGLE r, int i);

    float get_XR(RECTANGLE r);
    void set_xR(RECTANGLE r, float x);

    float get_YR(RECTANGLE r);
    void set_yR(RECTANGLE r, float y);

    char* get_cbR(RECTANGLE r);
    void set_cbR(RECTANGLE r, char* cb);

    char* get_cpR(RECTANGLE r);
    void set_cpR(RECTANGLE r, char* cp);

    float get_wR(RECTANGLE r);

    float get_hR(RECTANGLE r);

    float get_areaR(RECTANGLE r);

    void kill_rectangle(RECTANGLE r);


#endif
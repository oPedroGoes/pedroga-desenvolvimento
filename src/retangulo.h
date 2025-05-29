#ifndef retangulo_h
#define retangulo_h
    
    typedef void* RECTANGLE;


    RECTANGLE create_rectangle(int id, double x, double y, double w, double h, char* cb, char* cp);


    int get_idR(RECTANGLE r);
    void set_idR(RECTANGLE r, int id);

    double get_XR(RECTANGLE r);
    void set_xR(RECTANGLE r, double x);

    double get_YR(RECTANGLE r);
    void set_yR(RECTANGLE r, double y);

    char* get_cbR(RECTANGLE r);
    void set_cbR(RECTANGLE r, char* cb);

    char* get_cpR(RECTANGLE r);
    void set_cpR(RECTANGLE r, char* cp);

    double get_wR(RECTANGLE r);

    double get_hR(RECTANGLE r);

    double get_areaR(RECTANGLE r);

    void kill_rectangle(RECTANGLE r);


#endif
//===========================================================================
// training_data.h
//===========================================================================
// @brief: This header defines a 2D array that includes 10 training sets, 
//         where each set contains 1800 training data.


#ifndef TRAINING_DATA_H
#define TRAINING_DATA_H
 
#define TRAINING_SIZE 100

const char training_data[26][TRAINING_SIZE][785] = {
    {
    #include "chardata/training_set_a.dat" 
    }, 
    { 
    #include "chardata/training_set_b.dat" 
    },
    { 
    #include "chardata/training_set_c.dat" 
    },
    { 
    #include "chardata/training_set_d.dat" 
    },
    { 
    #include "chardata/training_set_e.dat" 
    },
    { 
    #include "chardata/training_set_f.dat" 
    },
    { 
    #include "chardata/training_set_g.dat" 
    },
    { 
    #include "chardata/training_set_h.dat" 
    },
    { 
    #include "chardata/training_set_i.dat" 
    },
    { 
    #include "chardata/training_set_j.dat"
    },
    { 
    #include "chardata/training_set_k.dat"
    },
    { 
    #include "chardata/training_set_l.dat"
    },
    { 
    #include "chardata/training_set_m.dat"
    },
    { 
    #include "chardata/training_set_n.dat"
    },
    { 
    #include "chardata/training_set_o.dat"
    },
    { 
    #include "chardata/training_set_p.dat"
    },
    { 
    #include "chardata/training_set_q.dat"
    },
    { 
    #include "chardata/training_set_r.dat"
    },
    { 
    #include "chardata/training_set_s.dat"
    },
    { 
    #include "chardata/training_set_t.dat"
    },
    { 
    #include "chardata/training_set_u.dat"
    },
    { 
    #include "chardata/training_set_v.dat"
    },
    { 
    #include "chardata/training_set_w.dat"
    },
    { 
    #include "chardata/training_set_x.dat"
    },
    { 
    #include "chardata/training_set_y.dat"
    },
    { 
    #include "chardata/training_set_z.dat"
    },
};

#endif
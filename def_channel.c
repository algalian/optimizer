#include "optimizer.h"


/* Order does NOT matter */
const t_column_spec channel_schema[] = {
    { "Channel", COL_STRING,  offsetof(t_channel, name),  true  },
    { "A",       COL_LDOUBLE, offsetof(t_channel, a),     true  },
    { "B",       COL_LDOUBLE, offsetof(t_channel, b),     true  },
    { "C",       COL_LDOUBLE, offsetof(t_channel, c),     true  },
    { "CPM",     COL_LDOUBLE, offsetof(t_channel, cpm),   true  },
    { NULL, 0, 0, false }
};

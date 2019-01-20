#include <iostream>
#define BOOST_TEST_MODULE shuffle_test
#include <boost/test/unit_test.hpp>


#include <vector>
#include "shuffle.hpp"

using namespace Shuffle;

BOOST_AUTO_TEST_CASE(shuffle_test_1) {
  {
    std::vector<unsigned char> msg(24);
    std::vector<unsigned char> shufmsg(24) ;
    std::vector<unsigned char> umsg(24);
    for(int i=0; i<24; ++i)
      msg[i]=i*10;
    shuffle<4>(msg.data(), shufmsg.data(), msg.size());
    unshuffle<4>(shufmsg.data(), umsg.data(), msg.size());
    std::vector<unsigned char> result_shuffle({0, 40, 80, 120,
                                               160, 200, 10, 50,
                                               90, 130, 170, 210,
                                               20, 60, 100, 140,
                                               180, 220, 30, 70,
                                               110, 150, 190,230});    
    for(int i=0; i<24; ++i) {
      BOOST_CHECK_EQUAL(shufmsg[i], result_shuffle[i]);
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }
  }
}

BOOST_AUTO_TEST_CASE(shuffle_test_2) {
  {
    std::vector<unsigned char> msg(24);
    std::vector<unsigned char> shufmsg(24) ;
    std::vector<unsigned char> umsg(24);
    for(int i=0; i<24; ++i)
      msg[i]=i*10;
    shuffle<16>(msg.data(), shufmsg.data(), msg.size());
    unshuffle<16>(shufmsg.data(), umsg.data(), msg.size());
    std::vector<unsigned char> result_shuffle({0, 10, 20, 30,
                                               40, 50, 60, 70,
                                               80, 90, 100, 110,
                                               120, 130, 140, 150,
                                               160, 170, 180, 190,
                                               200, 210, 220,230});    
    for(int i=0; i<24; ++i) {
      BOOST_CHECK_EQUAL(shufmsg[i], result_shuffle[i]);
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }
  }
}

BOOST_AUTO_TEST_CASE(shuffle_test_3) {
  {
    std::vector<unsigned char> msg(5000);
    for(int i=0; i<5000; ++i)
      msg[i]= (i*i) % 255;
    std::vector<unsigned char> shufmsg = shuffle(msg);
    std::vector<unsigned char> umsg = unshuffle(shufmsg);
    for(int i=0; i<5000; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }
  } 
}

BOOST_AUTO_TEST_CASE(shuffle_test_4) {
  {
    std::vector<unsigned char> msg(5000);
    for(int i=0; i<5000; ++i)
      msg[i]= (i*i) % 255;
    std::vector<unsigned char> shufmsg(5000);
    std::vector<unsigned char> umsg(5000);
    shuffle(reinterpret_cast<const unsigned char*>(msg.data()), 
            reinterpret_cast<unsigned char*>(shufmsg.data()), 5000, 17, 1024);
    unshuffle(reinterpret_cast<const unsigned char*>(shufmsg.data()), 
              reinterpret_cast<unsigned char*>(umsg.data()), 5000, 17, 1024);
    for(int i=0; i<5000; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }
  } 
}

BOOST_AUTO_TEST_CASE(shuffle_test_5) {
  {
    std::vector<double> msg(5000);
    for(int i=0; i<5000; ++i)
      msg[i]= (i*i) % 3782463827468;
    std::vector<double> shufmsg = shuffle(msg);
    std::vector<double> umsg = unshuffle(shufmsg);
    for(int i=0; i<5000; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }
  } 
}

BOOST_AUTO_TEST_CASE(shuffle_test_6) {
  {
    std::vector<long long> msg(5000);
    for(int i=0; i<5000; ++i)
      msg[i]= (i*i) % 3782463827468;
    std::vector<long long> shufmsg = shuffle(msg);
    std::vector<long long> umsg = unshuffle(shufmsg);
    for(int i=0; i<5000; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }
  } 
}

BOOST_AUTO_TEST_CASE(shuffle_test_7) {
  {
    std::vector<unsigned char> msg(11);
    for(int i=0; i<11; ++i)
      msg[i]= i;
    std::vector<unsigned char> shufmsg(11);
    std::vector<unsigned char> umsg(11);
    shuffle(reinterpret_cast<const unsigned char*>(msg.data()), 
            reinterpret_cast<unsigned char*>(shufmsg.data()), 11, 4, 1024);
    unshuffle(reinterpret_cast<const unsigned char*>(shufmsg.data()), 
              reinterpret_cast<unsigned char*>(umsg.data()), 11, 4, 1024);
    for(int i=0; i<11; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }
  } 
}


BOOST_AUTO_TEST_CASE(shuffle_test_8) {
  std::vector<long long> msg(5000);
  std::vector<long long> smsg_c(5000);
  std::vector<long long> umsg_c(5000);
  for(int i=0; i<5000; ++i)
    msg[i]= i;//(i*i) % 3782463827468;
  std::vector<long long> smsg = shuffle(msg);
  std::vector<long long> umsg = unshuffle(smsg);
  shuffle(reinterpret_cast<const unsigned char*>(msg.data()), 
            reinterpret_cast<unsigned char*>(smsg_c.data()), 5000*sizeof(long long), sizeof(long long));
  unshuffle(reinterpret_cast<const unsigned char*>(smsg.data()), 
            reinterpret_cast<unsigned char*>(umsg_c.data()), 5000*sizeof(long long), sizeof(long long));
  for(int i=0; i<10; ++i) {
    BOOST_CHECK_EQUAL(smsg[i], smsg_c[i]);
    BOOST_CHECK_EQUAL(umsg[i], umsg_c[i]);
    BOOST_CHECK_EQUAL(umsg[i], msg[i]);
  }
  
/*  
  
  std::vector<long long> umsg(5000);
  unshuffle(reinterpret_cast<const unsigned char*>(shufmsg.data()), 
            reinterpret_cast<unsigned char*>(umsg.data()), 5000*sizeof(long long), sizeof(long long));
  for(int i=0; i<5000; ++i) {
    std::cout << i << std::endl;
    BOOST_CHECK_EQUAL(umsg[i], msg[i]);
  }*/
}


BOOST_AUTO_TEST_CASE(shuffle_test_9) {
  {
    std::vector<unsigned char> msg(181984);
    std::vector<unsigned char> shufmsg(181984) ;
    std::vector<unsigned char> umsg(181984);
    for(int i=0; i<181984; ++i)
      msg[i]=i*10;
    shuffle<32>(msg.data(), shufmsg.data(), msg.size());
    unshuffle<32>(shufmsg.data(), umsg.data(), msg.size());
    for(int i=0; i<181984; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }
  }
}

BOOST_AUTO_TEST_CASE(shuffle_test_10) {
  {
    std::vector<unsigned char> msg(24);
    std::vector<unsigned char> shufmsg(24) ;
    std::vector<unsigned char> umsg(24);
    for(int i=0; i<24; ++i)
      msg[i]=i*10;
    shuffle<0>(msg.data(), shufmsg.data(), msg.size());
    unshuffle<0>(shufmsg.data(), umsg.data(), msg.size());
    for(int i=0; i<24; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }
  }
}

BOOST_AUTO_TEST_CASE(shuffle_test_11) {
  {
    std::vector<double> msg(5000);
    for(int i=0; i<5000; ++i)
      msg[i]= (i*i) % 3782463827468;
    {
      BOOST_CHECK_THROW(shuffle(msg, 0), std::logic_error);
      BOOST_CHECK_THROW(unshuffle(msg, 0), std::logic_error);
      BOOST_CHECK_THROW(shuffle(msg, 1), std::logic_error);
      BOOST_CHECK_THROW(unshuffle(msg, 1), std::logic_error);
    }
    {
      std::vector<unsigned char> shufmsg(5000);
      std::vector<unsigned char> umsg(5000);
      BOOST_CHECK_THROW(shuffle(reinterpret_cast<const unsigned char*>(msg.data()), 
              reinterpret_cast<unsigned char*>(shufmsg.data()), 11, 4, 3), std::logic_error);
      BOOST_CHECK_THROW(unshuffle(reinterpret_cast<const unsigned char*>(shufmsg.data()), 
                reinterpret_cast<unsigned char*>(umsg.data()), 11, 4, 3), std::logic_error);
    }
    {
      std::vector<double> shufmsg = shuffle(msg, 32);
      std::vector<double> umsg = unshuffle(shufmsg, 32);
      for(int i=0; i<5000; ++i) {
        BOOST_CHECK_EQUAL(umsg[i], msg[i]);
      }
    }
  } 
}


struct sz128 {
  int64_t  f1;
  int64_t  f2;
  
  bool operator== (const sz128& rhs) const { return f1 == rhs.f1 && f2 == rhs.f2; }
};
// LCOV_EXCL_START
std::ostream& operator<<(std::ostream& out, const sz128& v) { out << v.f1 << v.f2; return out; };
// LCOV_EXCL_STOP

struct sz80 {
  char  f1;
  char  f2;
  char  f3;
  char  f4;
  char  f5;
  char  f6;
  char  f7;
  char  f8;
  char  f9;
  char  f10;
  
  bool operator== (const sz80& rhs) const { return f1 == rhs.f1 && f2 == rhs.f2; }
};
// LCOV_EXCL_START
std::ostream& operator<<(std::ostream& out, const sz80& v) { out << v.f1 << v.f2; return out; };
// LCOV_EXCL_STOP


struct sz256 {
  int64_t  f1;
  int64_t  f2;
  int64_t  f3;
  int64_t  f4;
  
  bool operator== (const sz256& rhs) const { return f1 == rhs.f1 && f2 == rhs.f2 && f3 == rhs.f3 && f4 == rhs.f4; }
};
// LCOV_EXCL_START
std::ostream& operator<<(std::ostream& out, const sz256& v) { out << v.f1 << v.f2 << v.f3 << v.f4; return out; };
// LCOV_EXCL_STOP

struct sz512 {
  int64_t  f1;
  int64_t  f2;
  int64_t  f3;
  int64_t  f4;
  int64_t  f5;
  int64_t  f6;
  int64_t  f7;
  int64_t  f8;
  
  bool operator== (const sz512& rhs) const { return f1 == rhs.f1 && f2 == rhs.f2 && f3 == rhs.f3 && f4 == rhs.f4; }
};
// LCOV_EXCL_START
std::ostream& operator<<(std::ostream& out, const sz512& v) { out << v.f1 << v.f2 << v.f3 << v.f4; return out; };
// LCOV_EXCL_STOP


BOOST_AUTO_TEST_CASE(shuffle_test_12) {

  // size 1
  {
    std::vector<char> msg(5000);
    for(int i=0; i<5000; ++i)
      msg[i]= (i*i) % 3782463827468;
    std::vector<char> shufmsg = shuffle(msg, 32);
    std::vector<char> umsg = unshuffle(shufmsg, 32);
    for(int i=0; i<5000; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }  
  }  

  // size 2
  {
    std::vector<int16_t> msg(5000);
    for(int i=0; i<5000; ++i)
      msg[i]= (i*i) % 3782463827468;
    std::vector<int16_t> shufmsg = shuffle(msg, 32);
    std::vector<int16_t> umsg = unshuffle(shufmsg, 32);
    for(int i=0; i<5000; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }  
  }  

  // size 4
  {
    std::vector<int32_t> msg(5000);
    for(int i=0; i<5000; ++i)
      msg[i]= (i*i) % 3782463827468;
    std::vector<int32_t> shufmsg = shuffle(msg, 32);
    std::vector<int32_t> umsg = unshuffle(shufmsg, 32);
    for(int i=0; i<5000; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }  
  }  

  // size 8
  {
    std::vector<int64_t> msg(5000);
    for(int i=0; i<5000; ++i)
      msg[i]= (i*i) % 3782463827468;
    std::vector<int64_t> shufmsg = shuffle(msg, 32);
    std::vector<int64_t> umsg = unshuffle(shufmsg, 32);
    for(int i=0; i<5000; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }  
  }  
  
  // size 16
  {
    std::vector<sz128> msg(5000);
    for(int i=0; i<5000; ++i) {
      msg[i].f1 = (i*i) % 3782463827468;
      msg[i].f2 = (i*i) % 37824612323168;
    }
    std::vector<sz128> shufmsg = shuffle(msg, 32);
    std::vector<sz128> umsg = unshuffle(shufmsg, 32);
    for(int i=0; i<5000; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }  
  }  

  // size 32
  {
    std::vector<sz256> msg(5000);
    for(int i=0; i<5000; ++i) {
      msg[i].f1 = (i*i) % 3782463827468;
      msg[i].f2 = (i*i) % 37824612323168;
      msg[i].f3 = (i*i*i) % 37824612323168;
      msg[i].f4 = (i*i*i) % 37824612323168;
    }
    std::vector<sz256> shufmsg = shuffle(msg, 32);
    std::vector<sz256> umsg = unshuffle(shufmsg, 32);
    for(int i=0; i<5000; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }  
  }  

  // size 64
  {
    std::vector<sz512> msg(5000);
    for(int i=0; i<5000; ++i) {
      msg[i].f1 = (i*i) % 3782463827468;
      msg[i].f2 = (i*i) % 37824612323168;
      msg[i].f3 = (i*i*i) % 37824612323168;
      msg[i].f4 = (i*i*i) % 37824612323168;
      msg[i].f5 = (i*i*i) % 37824612323168;
      msg[i].f6 = (i*i*i) % 37824612323168;
      msg[i].f7 = (i*i*i) % 37824612323168;
      msg[i].f8 = (i*i*i) % 37824612323168;
    }
    std::vector<sz512> shufmsg = shuffle(msg, 128);
    std::vector<sz512> umsg = unshuffle(shufmsg, 128);
    for(int i=0; i<5000; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }  
  }  
  
  
  
  
  // other
  {
    std::vector<sz80> msg(5000);
    for(int i=0; i<5000; ++i) {
      msg[i].f1 = (i*i) % 3782463827468;
      msg[i].f2 = (i*i) % 37824612323168;
    }
    std::vector<sz80> shufmsg = shuffle(msg, 32);
    std::vector<sz80> umsg = unshuffle(shufmsg, 32);
    for(int i=0; i<5000; ++i) {
      BOOST_CHECK_EQUAL(umsg[i], msg[i]);
    }  
  }  
  
}

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
    for(int i=0; i<24; ++i)
      msg[i]=i*10;
    std::vector<unsigned char> shufmsg = shuffle<4>(msg);
    std::vector<unsigned char> umsg = unshuffle<4>(shufmsg);
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

BOOST_AUTO_TEST_CASE(shuffle_test_3) {
  {
    std::vector<unsigned char> msg(5000);
    for(int i=0; i<5000; ++i)
      msg[i]= (i*i) % 255;
    std::vector<unsigned char> shufmsg = shuffle<16>(msg);
    std::vector<unsigned char> umsg = unshuffle<16>(shufmsg);
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
    std::vector<unsigned char> shufmsg = shuffle<17>(msg);
    std::vector<unsigned char> umsg = unshuffle<17>(shufmsg);
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
//     std::vector<unsigned char> umsg = unshuffle<17>(shufmsg);
//     for(int i=0; i<5000; ++i) {
//       BOOST_CHECK_EQUAL(umsg[i], msg[i]);
//     }
  } 
}

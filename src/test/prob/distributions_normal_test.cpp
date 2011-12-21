#include <gtest/gtest.h>
#include "stan/agrad/agrad.hpp"
#include "stan/meta/traits.hpp"
#include "stan/prob/distributions_normal.hpp"


using boost::math::policies::policy;
using boost::math::policies::evaluation_error;
using boost::math::policies::domain_error;
using boost::math::policies::overflow_error;
using boost::math::policies::domain_error;
using boost::math::policies::pole_error;
using boost::math::policies::errno_on_error;

typedef policy<
  domain_error<errno_on_error>, 
  pole_error<errno_on_error>,
  overflow_error<errno_on_error>,
  evaluation_error<errno_on_error> 
  > errno_policy;


TEST(ProbDistributions,Normal) {
  // values from R dnorm()
  EXPECT_FLOAT_EQ(-0.9189385, stan::prob::normal_log(0.0,0.0,1.0));
  EXPECT_FLOAT_EQ(-1.418939,  stan::prob::normal_log(1.0,0.0,1.0));
  EXPECT_FLOAT_EQ(-2.918939,  stan::prob::normal_log(-2.0,0.0,1.0));
  EXPECT_FLOAT_EQ(-3.174270,  stan::prob::normal_log(-3.5,1.9,7.2));
}
TEST(ProbDistributionsDefaultPolicy,NormalScale) {
  double sigma_d = 0.0;
  EXPECT_THROW(stan::prob::normal_log(0.0,0.0,sigma_d), std::domain_error);
  sigma_d = -1.0;
  EXPECT_THROW(stan::prob::normal_log(0.0,0.0,sigma_d), std::domain_error);
}
TEST(ProbDistributionsErrnoPolicy,NormalScale) {
  double sigma_d = 0.0;
  double result = 0;
  
  result = stan::prob::normal_log(0.0,0.0,sigma_d, errno_policy());
  EXPECT_TRUE (std::isnan (result));
  
  sigma_d = -1.0;
  result = stan::prob::normal_log(0.0,0.0,sigma_d, errno_policy());
  EXPECT_TRUE (std::isnan (result));
}
TEST(ProbDistributionsDefaultPolicy,NormalY) {
  double y = 0.0;
  EXPECT_NO_THROW (stan::prob::normal_log(y,0.0,1.0));
  
  y = std::numeric_limits<double>::quiet_NaN();
  EXPECT_THROW(stan::prob::normal_log(y,0.0,1.0), std::domain_error);
  y = std::numeric_limits<double>::infinity();
  EXPECT_THROW(stan::prob::normal_log(y,0.0,1.0), std::domain_error);
  y = -std::numeric_limits<double>::infinity();
  EXPECT_THROW(stan::prob::normal_log(y,0.0,1.0), std::domain_error);
}
TEST(ProbDistributionsErrnoPolicy,NormalY) {
  double result = 0;
  double y = 0.0;
  EXPECT_NO_THROW (result = stan::prob::normal_log(y,0.0,1.0,errno_policy()));
  
  y = std::numeric_limits<double>::quiet_NaN();
  result = stan::prob::normal_log(y,0.0,1.0,errno_policy());
  EXPECT_TRUE (std::isnan(result));
  
  y = std::numeric_limits<double>::infinity();
  result = stan::prob::normal_log(y,0.0,1.0,errno_policy());
  EXPECT_TRUE (std::isnan(result));

  y = -std::numeric_limits<double>::infinity();
  result = stan::prob::normal_log(y,0.0,1.0,errno_policy());
  EXPECT_TRUE (std::isnan(result));
}
TEST(ProbDistributionsDefaultPolicy,NormalLocation) {
  double mu = 0.0;
  EXPECT_NO_THROW (stan::prob::normal_log(0.0,mu,1.0));
  
  mu = std::numeric_limits<double>::quiet_NaN();
  EXPECT_THROW(stan::prob::normal_log(0.0,mu,1.0), std::domain_error);
  mu = std::numeric_limits<double>::infinity();
  EXPECT_THROW(stan::prob::normal_log(0.0,mu,1.0), std::domain_error);
  mu = -std::numeric_limits<double>::infinity();
  EXPECT_THROW(stan::prob::normal_log(0.0,mu,1.0), std::domain_error);
}
TEST(ProbDistributionsErrnoPolicy,NormalLocation) {
  double result = 0;
  double mu = 0.0;
  EXPECT_NO_THROW (result = stan::prob::normal_log(0.0,mu,1.0,errno_policy()));
  
  mu = std::numeric_limits<double>::quiet_NaN();
  result = stan::prob::normal_log(0.0,mu,1.0,errno_policy());
  EXPECT_TRUE (std::isnan(result));
  
  mu = std::numeric_limits<double>::infinity();
  result = stan::prob::normal_log(0.0,mu,1.0,errno_policy());
  EXPECT_TRUE (std::isnan(result));

  mu = -std::numeric_limits<double>::infinity();
  result = stan::prob::normal_log(0.0,mu,1.0,errno_policy());
  EXPECT_TRUE (std::isnan(result));
}
TEST(ProbDistributionsPropTo,Normal) {
  EXPECT_FLOAT_EQ(0, stan::prob::normal_log<true>(0.0,0.0,1.0));
  EXPECT_FLOAT_EQ(0, stan::prob::normal_log<true>(1.0,0.0,1.0));
  EXPECT_FLOAT_EQ(0, stan::prob::normal_log<true>(-2.0,0.0,1.0));
  EXPECT_FLOAT_EQ(0, stan::prob::normal_log<true>(-3.5,1.9,7.2));
}
TEST(ProbDistributionsCumulative,Normal) {
  EXPECT_FLOAT_EQ(0.5000000, stan::prob::normal_p(0.0, 0.0, 1.0));
  EXPECT_FLOAT_EQ(0.8413447, stan::prob::normal_p(1.0, 0.0, 1.0));
  EXPECT_FLOAT_EQ(0.4012937, stan::prob::normal_p(1.0, 2.0, 4.0));
}
TEST(ProbDistributionsCumulativeDefaultPolicy,NormalSigma) {
  // sigma = 0
  EXPECT_THROW(stan::prob::normal_p(0.0, 0.0, 0.0), std::domain_error);
  // sigma = -1
  EXPECT_THROW(stan::prob::normal_p(0.0, 0.0, -1.0), std::domain_error);  
}
TEST(ProbDistributions,NormalVector) {
  double mu = -2.9;
  double sigma = 1.7;

  std::vector<double> x;
  EXPECT_FLOAT_EQ(0.0, stan::prob::normal_log(x,mu,sigma));

  x.push_back(-2.0);
  x.push_back(-1.5);
  x.push_back(0.0);
  x.push_back(12.0);
  
  EXPECT_FLOAT_EQ(-46.14256, stan::prob::normal_log(x,mu,sigma));
}
TEST(ProbDistributionsPropTo,NormalVector) {
  std::vector<double> x;
  EXPECT_FLOAT_EQ(0.0, stan::prob::normal_log<true>(x,0.0,1.0));

  x.push_back(-2.0);
  x.push_back(-1.5);
  x.push_back(0.0);
  x.push_back(12.0);

  EXPECT_FLOAT_EQ(0.0, stan::prob::normal_log<true>(x,0.0,1.0));
  EXPECT_FLOAT_EQ(0.0, stan::prob::normal_log<true>(x,0.0,1.0));
  EXPECT_FLOAT_EQ(0.0, stan::prob::normal_log<true>(x,0.0,1.0));
  EXPECT_FLOAT_EQ(0.0, stan::prob::normal_log<true>(x,1.9,7.2));
}
TEST(ProbDistributionsDefaultPolicy,NormalVectorScale) {
  std::vector<double> x;
  x.push_back(-2.0);
  x.push_back(-1.5);
  x.push_back(0.0);
  x.push_back(12.0);

  double sigma_d = 0.0;
  EXPECT_THROW(stan::prob::normal_log(x,0.0,sigma_d), std::domain_error);
  sigma_d = -1.0;
  EXPECT_THROW(stan::prob::normal_log(x,0.0,sigma_d), std::domain_error);
}
TEST(ProbDistributionsErrnoPolicy,NormalVectorScale) {
  std::vector<double> x;
  x.push_back(-2.0);
  x.push_back(-1.5);
  x.push_back(0.0);
  x.push_back(12.0);
  
  double result = 0;
  
  double sigma_d = 0.0;
  result = stan::prob::normal_log(x, 0.0, sigma_d, errno_policy());
  EXPECT_TRUE (std::isnan(result));
  
  sigma_d = -1.0;
  result = stan::prob::normal_log(x, 0.0, sigma_d, errno_policy());
  EXPECT_TRUE (std::isnan(result));
}
TEST(ProbDistributionsDefaultPolicy,NormalVectorY) {
  std::vector<double> y;
  y.push_back(-2.0);
  y.push_back(-1.5);
  y.push_back(0.0);
  y.push_back(12.0);

  EXPECT_NO_THROW (stan::prob::normal_log(y,0.0,1.0));
  
  y[1] = std::numeric_limits<double>::quiet_NaN();
  EXPECT_THROW(stan::prob::normal_log(y,0.0,1.0), std::domain_error);
  y[1] = std::numeric_limits<double>::infinity();
  EXPECT_THROW(stan::prob::normal_log(y,0.0,1.0), std::domain_error);
  y[1] = -std::numeric_limits<double>::infinity();
  EXPECT_THROW(stan::prob::normal_log(y,0.0,1.0), std::domain_error);
}
TEST(ProbDistributionsErrnoPolicy,NormalVectorY) {
  double result = 0;
  std::vector<double> y;
  y.push_back(-2.0);
  y.push_back(-1.5);
  y.push_back(0.0);
  y.push_back(12.0);
  EXPECT_NO_THROW (result = stan::prob::normal_log(y,0.0,1.0,errno_policy()));
  
  y[2] = std::numeric_limits<double>::quiet_NaN();
  result = stan::prob::normal_log(y,0.0,1.0,errno_policy());
  EXPECT_TRUE (std::isnan(result));
  
  y[2] = std::numeric_limits<double>::infinity();
  result = stan::prob::normal_log(y,0.0,1.0,errno_policy());
  EXPECT_TRUE (std::isnan(result));

  y[2] = -std::numeric_limits<double>::infinity();
  result = stan::prob::normal_log(y,0.0,1.0,errno_policy());
  EXPECT_TRUE (std::isnan(result));
}
TEST(ProbDistributionsDefaultPolicy,NormalVectorLocation) {
  std::vector<double> y;
  y.push_back(-2.0);
  y.push_back(-1.5);
  y.push_back(0.0);
  y.push_back(12.0);

  double mu = 0.0;
  EXPECT_NO_THROW (stan::prob::normal_log(y,mu,1.0));
  
  mu = std::numeric_limits<double>::quiet_NaN();
  EXPECT_THROW(stan::prob::normal_log(y,mu,1.0), std::domain_error);
  mu = std::numeric_limits<double>::infinity();
  EXPECT_THROW(stan::prob::normal_log(y,mu,1.0), std::domain_error);
  mu = -std::numeric_limits<double>::infinity();
  EXPECT_THROW(stan::prob::normal_log(y,mu,1.0), std::domain_error);
}
TEST(ProbDistributionsErrnoPolicy,NormalVectorLocation) {
  double result = 0;
  double mu = 0.0;
  std::vector<double> y;
  y.push_back(-2.0);
  y.push_back(-1.5);
  y.push_back(0.0);
  y.push_back(12.0);
  EXPECT_NO_THROW (result = stan::prob::normal_log(y,mu,1.0,errno_policy()));
  
  mu = std::numeric_limits<double>::quiet_NaN();
  result = stan::prob::normal_log(y,mu,1.0,errno_policy());
  EXPECT_TRUE (std::isnan(result));
  
  mu = std::numeric_limits<double>::infinity();
  result = stan::prob::normal_log(y,mu,1.0,errno_policy());
  EXPECT_TRUE (std::isnan(result));

  mu = -std::numeric_limits<double>::infinity();
  result = stan::prob::normal_log(y,mu,1.0,errno_policy());
  EXPECT_TRUE (std::isnan(result));
}



void expect_eq_diffs(double x1, double x2, 
		     double y1, double y2) {
  EXPECT_FLOAT_EQ(x1-x2,y1-y2);
}

void expect_eq_diffs(const stan::agrad::var& x1, 
		     const stan::agrad::var& x2,
		     const stan::agrad::var& y1, 
		     const stan::agrad::var& y2) {
  expect_eq_diffs(x1.val(), x2.val(), 
		  y1.val(), y2.val());
}

template <typename T_y, typename T_loc, typename T_scale>
void expect_propto(T_y y1, T_loc mu1, T_scale sigma1,
		   T_y y2, T_loc mu2, T_scale sigma2) {
  expect_eq_diffs(stan::prob::normal_log<false>(y1,mu1,sigma1),
		  stan::prob::normal_log<false>(y2,mu2,sigma2),
		  stan::prob::normal_log<true>(y1,mu1,sigma1),
		  stan::prob::normal_log<true>(y2,mu2,sigma2));
}

using stan::agrad::var;

TEST(distributionsNormal,propto) {
  // d,d,d
  expect_propto<double,double,double>(1.0,2.0,3.0, 
				      1.0,2.0,3.0);
  // d,d,v
  expect_propto<double,double,var>(1.0,2.0,3.0, 
				   1.0,2.0,4.0);
  // d,v,d
  expect_propto<double,var,double>(1.0,2.0,3.0, 
				   1.0,4.0,3.0);
  // d,v,v
  expect_propto<double,var,var>(1.0,2.0,3.0, 
				1.0,-1.0,1.0);
  // v,d,d
  expect_propto<var,double,double>(1.0,2.0,3.0, 
				   3.0,17.0,3.0);
  // v,d,v
  expect_propto<var,double,var>(1.0,2.0,3.0, 
				4.0,2.0,1.0);
  // v,v,d
  expect_propto<var,var,double>(1.0,2.0,3.0, 
				2.0,-4.0,3.0);
  // v,v,v
  expect_propto<var,var,var>(1.0,2.0,3.0, 
			     5.0,7.0,1.5);
}


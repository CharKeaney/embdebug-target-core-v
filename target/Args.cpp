// Definition of a class to process command line arguments.
//
// This file is part of the Embecosm Debug Server target for CORE-V MCU
//
// Copyright (C) 2021 Embecosm Limited
// SPDX-License-Identifier: Apache-2.0

#include <iostream>
#include <string>
#include <vector>

#include "Args.h"
#include "cxxopts.hpp"

using cxxopts::OptionException;
using cxxopts::Options;
using cxxopts::ParseResult;
using cxxopts::value;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

/// \brief Constructor.
///
/// Parse the arguments to create all the information needed.
///
/// \param [in] argc  Number of command line arguments.
/// \param [in] argv  Vector of the command line arguments.
Args::Args (int argc, char *argv[])
{
  Options options ("embdebug-target-core-v", "Embdebug CORE-V target library");
  options.add_options () ("s,mhz", "Clock speed in MHz",
                          value<double> ()->default_value ("100"), "<speed>");
  options.add_options () ("d,duration-ns", "Simulation duration in nanoseconds",
                          value<uint64_t> ()->default_value ("0"), "<time>");
  options.add_options () ("vcd", "Verilog Change Dump file name",
                          value<string> ()->default_value (""), "<filename>");
  options.add_options () ("test-status", "Run a test of hart status");
  options.add_options () ("test-gprs", "Run a test of the GPRs");
  options.add_options () ("test-fprs", "Run a test of the FPRs");
  options.add_options () ("test-csrs", "Run a test of the CSRs");
  options.add_options () ("h,help", "Produce help message and exit");
  options.add_options () ("v,version", "Produce version message and exit");

  ParseResult res;
  try
    {
      res = options.parse (argc, argv);
    }
  catch (OptionException &e)
    {
      cerr << "ERROR: unable to parse arguments:" << e.what () << endl;
      cerr << options.help ();
      exit (EXIT_FAILURE);
    }

  if (res.count ("help") > 0)
    {
      cout << options.help ();
      exit (EXIT_SUCCESS);
    }

  if (res.count ("version") > 0)
    {
      cout << "embdebug-target-core-v version 0.0.0" << endl;
      exit (EXIT_SUCCESS);
    }

  double mhzVal = res["mhz"].as<double> ();

  if (mhzVal > 500.0)
    {
      cerr << "ERROR: speed cannot be greater than 500MHz" << endl;
      exit (EXIT_FAILURE);
    }

  mClkPeriodNs = static_cast<uint64_t> (1000.0 / mhzVal);

  mDurationNs = res["duration-ns"].as<uint64_t> ();

  mVcd = res["vcd"].as<string> ();

  // If the filename does not end in .vcd or .VCD, then add the suffix.
  if (mVcd.size () > 4)
    {
      vector<string> keys = { ".vcd", ".VCD" };
      for (auto const &k : keys)
        {
          std::size_t end_pos = mVcd.size () - k.size ();
          if (mVcd.rfind (k, end_pos) == end_pos)
            return;
        }
    }

  if (!mVcd.empty ())
    {
      mVcd.append (".vcd");
    }

  mTestStatus = res.count ("test-status") > 0;
  mTestGprs = res.count ("test-gprs") > 0;
  mTestFprs = res.count ("test-fprs") > 0;
  mTestCsrs = res.count ("test-csrs") > 0;
}

/// \brief Destructor.
///
/// For now nothing to do.
Args::~Args ()
{
}

/// \brief Getter for any VCD filename.
///
/// \return The filename for any VCD, the empty string if none were specified.
std::string
Args::vcd () const
{
  return mVcd;
}

/// \brief Getter for the clock period in nanoseconds.
///
/// \return The clock period in nanoseconds.
uint64_t
Args::clkPeriodNs () const
{
  return mClkPeriodNs;
}

/// \brief Getter for the run duration in nanoseconds.
///
/// \return The duration in nanoseconds.
uint64_t
Args::durationNs () const
{
  return mDurationNs;
}

/// \brief Getter for whether to test hart status
///
/// \return \c true if we should test hart status, \c false otherwise.
bool
Args::testStatus () const
{
  return mTestStatus;
}

/// \brief Getter for whether to test GPRs
///
/// \return \c true if we should test GPRs, \c false otherwise.
bool
Args::testGprs () const
{
  return mTestGprs;
}

/// \brief Getter for whether to test FPRs
///
/// \return \c true if we should test FPRs, \c false otherwise.
bool
Args::testFprs () const
{
  return mTestFprs;
}

/// \brief Getter for whether to test CSRs
///
/// \return \c true if we should test CSRs, \c false otherwise.
bool
Args::testCsrs () const
{
  return mTestCsrs;
}

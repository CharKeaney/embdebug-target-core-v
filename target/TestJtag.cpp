// Definition of the JTAG Test Suite
//
// This file is part of the Embecosm Debug Server target for CORE-V MCU
//
// Copyright (C) 2021 Embecosm Limited
// SPDX-License-Identifier: Apache-2.0

#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "TestJtag.h"
#include "Utils.h"

using std::cerr;
using std::cout;
using std::endl;
using std::memset;
using std::ostringstream;
using std::size_t;
using std::string;
using std::unique_ptr;
using std::vector;

// Instantiate the standard user CSR list
const vector<uint16_t> TestJtag::userCsrList = {
  Dmi::Csr::FFLAGS,        Dmi::Csr::FRM,           Dmi::Csr::FCSR,
  Dmi::Csr::CYCLE,         Dmi::Csr::INSTRET,       Dmi::Csr::HPMCOUNTER3,
  Dmi::Csr::HPMCOUNTER4,   Dmi::Csr::HPMCOUNTER5,   Dmi::Csr::HPMCOUNTER6,
  Dmi::Csr::HPMCOUNTER7,   Dmi::Csr::HPMCOUNTER8,   Dmi::Csr::HPMCOUNTER9,
  Dmi::Csr::HPMCOUNTER10,  Dmi::Csr::HPMCOUNTER11,  Dmi::Csr::HPMCOUNTER12,
  Dmi::Csr::HPMCOUNTER13,  Dmi::Csr::HPMCOUNTER14,  Dmi::Csr::HPMCOUNTER15,
  Dmi::Csr::HPMCOUNTER16,  Dmi::Csr::HPMCOUNTER17,  Dmi::Csr::HPMCOUNTER18,
  Dmi::Csr::HPMCOUNTER19,  Dmi::Csr::HPMCOUNTER20,  Dmi::Csr::HPMCOUNTER21,
  Dmi::Csr::HPMCOUNTER22,  Dmi::Csr::HPMCOUNTER23,  Dmi::Csr::HPMCOUNTER24,
  Dmi::Csr::HPMCOUNTER25,  Dmi::Csr::HPMCOUNTER26,  Dmi::Csr::HPMCOUNTER27,
  Dmi::Csr::HPMCOUNTER28,  Dmi::Csr::HPMCOUNTER29,  Dmi::Csr::HPMCOUNTER30,
  Dmi::Csr::HPMCOUNTER31,  Dmi::Csr::CYCLEH,        Dmi::Csr::INSTRETH,
  Dmi::Csr::HPMCOUNTERH3,  Dmi::Csr::HPMCOUNTERH4,  Dmi::Csr::HPMCOUNTERH5,
  Dmi::Csr::HPMCOUNTERH6,  Dmi::Csr::HPMCOUNTERH7,  Dmi::Csr::HPMCOUNTERH8,
  Dmi::Csr::HPMCOUNTERH9,  Dmi::Csr::HPMCOUNTERH10, Dmi::Csr::HPMCOUNTERH11,
  Dmi::Csr::HPMCOUNTERH12, Dmi::Csr::HPMCOUNTERH13, Dmi::Csr::HPMCOUNTERH14,
  Dmi::Csr::HPMCOUNTERH15, Dmi::Csr::HPMCOUNTERH16, Dmi::Csr::HPMCOUNTERH17,
  Dmi::Csr::HPMCOUNTERH18, Dmi::Csr::HPMCOUNTERH19, Dmi::Csr::HPMCOUNTERH20,
  Dmi::Csr::HPMCOUNTERH21, Dmi::Csr::HPMCOUNTERH22, Dmi::Csr::HPMCOUNTERH23,
  Dmi::Csr::HPMCOUNTERH24, Dmi::Csr::HPMCOUNTERH25, Dmi::Csr::HPMCOUNTERH26,
  Dmi::Csr::HPMCOUNTERH27, Dmi::Csr::HPMCOUNTERH28, Dmi::Csr::HPMCOUNTERH29,
  Dmi::Csr::HPMCOUNTERH30, Dmi::Csr::HPMCOUNTERH31
};

// Instantiate the custom user CSR list
const vector<uint16_t> TestJtag::customUserCsrList
    = { Dmi::Csr::LPSTART0, Dmi::Csr::LPEND0, Dmi::Csr::LPCOUNT0,
        Dmi::Csr::LPSTART1, Dmi::Csr::LPEND1, Dmi::Csr::LPCOUNT1,
        Dmi::Csr::UHARTID,  Dmi::Csr::PRIVLV };

// Instantiate the standard machine CSR list
const vector<uint16_t> TestJtag::machineCsrList = {
  Dmi::Csr::MSTATUS,        Dmi::Csr::MISA,           Dmi::Csr::MIE,
  Dmi::Csr::MTVEC,          Dmi::Csr::MCOUNTINHIBIT,  Dmi::Csr::MHPMEVENT3,
  Dmi::Csr::MHPMEVENT4,     Dmi::Csr::MHPMEVENT5,     Dmi::Csr::MHPMEVENT6,
  Dmi::Csr::MHPMEVENT7,     Dmi::Csr::MHPMEVENT8,     Dmi::Csr::MHPMEVENT9,
  Dmi::Csr::MHPMEVENT10,    Dmi::Csr::MHPMEVENT11,    Dmi::Csr::MHPMEVENT12,
  Dmi::Csr::MHPMEVENT13,    Dmi::Csr::MHPMEVENT14,    Dmi::Csr::MHPMEVENT15,
  Dmi::Csr::MHPMEVENT16,    Dmi::Csr::MHPMEVENT17,    Dmi::Csr::MHPMEVENT18,
  Dmi::Csr::MHPMEVENT19,    Dmi::Csr::MHPMEVENT20,    Dmi::Csr::MHPMEVENT21,
  Dmi::Csr::MHPMEVENT22,    Dmi::Csr::MHPMEVENT23,    Dmi::Csr::MHPMEVENT24,
  Dmi::Csr::MHPMEVENT25,    Dmi::Csr::MHPMEVENT26,    Dmi::Csr::MHPMEVENT27,
  Dmi::Csr::MHPMEVENT28,    Dmi::Csr::MHPMEVENT29,    Dmi::Csr::MHPMEVENT30,
  Dmi::Csr::MHPMEVENT31,    Dmi::Csr::MSCRATCH,       Dmi::Csr::MEPC,
  Dmi::Csr::MCAUSE,         Dmi::Csr::MTVAL,          Dmi::Csr::MIP,
  Dmi::Csr::TSELECT,        Dmi::Csr::TDATA1,         Dmi::Csr::TDATA2,
  Dmi::Csr::TDATA3,         Dmi::Csr::TINFO,          Dmi::Csr::MCONTEXT,
  Dmi::Csr::SCONTEXT,       Dmi::Csr::DCSR,           Dmi::Csr::DPC,
  Dmi::Csr::DSCRATCH0,      Dmi::Csr::DSCRATCH1,      Dmi::Csr::MCYCLE,
  Dmi::Csr::MINSTRET,       Dmi::Csr::MHPMCOUNTER3,   Dmi::Csr::MHPMCOUNTER4,
  Dmi::Csr::MHPMCOUNTER5,   Dmi::Csr::MHPMCOUNTER6,   Dmi::Csr::MHPMCOUNTER7,
  Dmi::Csr::MHPMCOUNTER8,   Dmi::Csr::MHPMCOUNTER9,   Dmi::Csr::MHPMCOUNTER10,
  Dmi::Csr::MHPMCOUNTER11,  Dmi::Csr::MHPMCOUNTER12,  Dmi::Csr::MHPMCOUNTER13,
  Dmi::Csr::MHPMCOUNTER14,  Dmi::Csr::MHPMCOUNTER15,  Dmi::Csr::MHPMCOUNTER16,
  Dmi::Csr::MHPMCOUNTER17,  Dmi::Csr::MHPMCOUNTER18,  Dmi::Csr::MHPMCOUNTER19,
  Dmi::Csr::MHPMCOUNTER20,  Dmi::Csr::MHPMCOUNTER21,  Dmi::Csr::MHPMCOUNTER22,
  Dmi::Csr::MHPMCOUNTER23,  Dmi::Csr::MHPMCOUNTER24,  Dmi::Csr::MHPMCOUNTER25,
  Dmi::Csr::MHPMCOUNTER26,  Dmi::Csr::MHPMCOUNTER27,  Dmi::Csr::MHPMCOUNTER28,
  Dmi::Csr::MHPMCOUNTER29,  Dmi::Csr::MHPMCOUNTER30,  Dmi::Csr::MHPMCOUNTER31,
  Dmi::Csr::MCYCLEH,        Dmi::Csr::MINSTRETH,      Dmi::Csr::MHPMCOUNTERH3,
  Dmi::Csr::MHPMCOUNTERH4,  Dmi::Csr::MHPMCOUNTERH5,  Dmi::Csr::MHPMCOUNTERH6,
  Dmi::Csr::MHPMCOUNTERH7,  Dmi::Csr::MHPMCOUNTERH8,  Dmi::Csr::MHPMCOUNTERH9,
  Dmi::Csr::MHPMCOUNTERH10, Dmi::Csr::MHPMCOUNTERH11, Dmi::Csr::MHPMCOUNTERH12,
  Dmi::Csr::MHPMCOUNTERH13, Dmi::Csr::MHPMCOUNTERH14, Dmi::Csr::MHPMCOUNTERH15,
  Dmi::Csr::MHPMCOUNTERH16, Dmi::Csr::MHPMCOUNTERH17, Dmi::Csr::MHPMCOUNTERH18,
  Dmi::Csr::MHPMCOUNTERH19, Dmi::Csr::MHPMCOUNTERH20, Dmi::Csr::MHPMCOUNTERH21,
  Dmi::Csr::MHPMCOUNTERH22, Dmi::Csr::MHPMCOUNTERH23, Dmi::Csr::MHPMCOUNTERH24,
  Dmi::Csr::MHPMCOUNTERH25, Dmi::Csr::MHPMCOUNTERH26, Dmi::Csr::MHPMCOUNTERH27,
  Dmi::Csr::MHPMCOUNTERH28, Dmi::Csr::MHPMCOUNTERH29, Dmi::Csr::MHPMCOUNTERH30,
  Dmi::Csr::MHPMCOUNTERH31, Dmi::Csr::MVENDORID,      Dmi::Csr::MARCHID,
  Dmi::Csr::MIMPID,         Dmi::Csr::MHARTID
};

/// Instantiate the output string stream.
ostringstream TestJtag::sOss;

/// \brief Constructor for the JTAG test suite
///
/// Initialize the random number generator
///
/// \brief dmi_       Reference to the DMI we are using
/// \brief numHarts_  Number of harts available to this debug interface.
/// \brief seed_      Seed for the random number generator.
TestJtag::TestJtag (std::unique_ptr<Dmi> &dmi_, uint32_t numHarts_,
                    uint32_t seed_)
    : mDmi (dmi_), mNumHarts (numHarts_), mDmstatus (dmi_->dmstatus ()),
      mHartinfo (dmi_->hartinfo ()), mAbstractcs (dmi_->abstractcs ()),
      mSbcs (dmi_->sbcs ())
{
  srand (seed_);
}

/// \brief Read \c hartinfo and \c abstractcs for a hart.
///
/// \param[in] h  The hart to read.
void
TestJtag::hartStatus (uint32_t h)
{
  mDmi->selectHart (h);

  mSbcs->reset ();
  mHartinfo->read ();
  mHartinfo->prettyPrint (false);
  cout << "hartinfo: 0x" << mHartinfo;
  mHartinfo->prettyPrint (true);
  cout << " " << mHartinfo << endl;

  mSbcs->reset ();
  mAbstractcs->read ();
  mAbstractcs->prettyPrint (false);
  cout << "abstractcs: 0x" << mAbstractcs;
  mAbstractcs->prettyPrint (true);
  cout << " " << mAbstractcs << endl;

  mSbcs->reset ();
  mSbcs->read ();
  mSbcs->prettyPrint (false);
  cout << "sbcs: 0x" << mSbcs;
  mSbcs->prettyPrint (true);
  cout << " " << mSbcs << endl << endl << endl;
}

/// \brief Report the PC for the currently selected hart
///
/// \note There is no option to select a different hart.
void
TestJtag::reportPC ()
{
  uint32_t pc;
  Dmi::Abstractcs::CmderrVal err = mDmi->readCsr (Dmi::Csr::DPC, pc);

  if (err == Dmi::Abstractcs::CMDERR_NONE)
    cout << "PC = 0x" << Utils::hexStr (pc) << endl;
  else
    cout << "PC read error " << err << endl;
}

/// \brief Halt the specified hart and report its status
///
/// \note This will also select the specified hart
///
/// \param[in] h  The hart to halt.
void
TestJtag::haltHart (uint32_t h)
{
  mDmi->haltHart (0);

  mDmstatus->read ();
  if (!mDmstatus->halted ())
    {
      mDmstatus->prettyPrint (false);
      cout << "Hart " << h << " failed to halt, dmstatus: 0x" << mDmstatus;
      mDmstatus->prettyPrint (true);
      cout << " " << mDmstatus << endl;
    }
}

/// \brief Test all the GPRs
///
/// \note There is no option to select a hart, it is presumed selected prior to
/// this.
void
TestJtag::testGprs ()
{
  uint32_t regval[0x20];

  // First report all the GPRs, saving the current value.
  cout << "Reading GPR values:" << endl;
  for (size_t r = 0; r < 32; r++)
    {
      string regName = fullGprName (r);
      Dmi::Abstractcs::CmderrVal err = mDmi->readGpr (r, regval[r]);

      if ((r % REGS_PER_ROW) == 0)
        cout << "  ";

      if (err == Dmi::Abstractcs::CMDERR_NONE)
        cout << regName << Utils::padStr (regName, 10) << " = "
             << Utils::hexStr (regval[r]);
      else
        cout << regName << Utils::padStr (regName, 10) << " " << err;

      if (((r % REGS_PER_ROW) == (REGS_PER_ROW - 1)) | (r == 31))
        cout << endl;
      else
        cout << "  ";
    }

  // Try to write and read back the registers.
  cout << "Writing GPR values:" << endl;
  bool succeeded = true;
  for (size_t r = 0; r < 32; r++)
    {
      uint32_t testvals[] = { 0xffffffff, 0x00000000, 0xdeadbeef, regval[r] };

      for (size_t i = 0; i < (sizeof (testvals) / sizeof (testvals[0])); i++)
        {
          Dmi::Abstractcs::CmderrVal err = mDmi->writeGpr (r, testvals[i]);

          if (err != Dmi::Abstractcs::CMDERR_NONE)
            {
              string regName = fullGprName (r);
              cout << regName << ": " << Utils::padStr (regName, 10)
                   << "Write error: " << err << endl;
              break;
            }

          uint32_t rval;
          err = mDmi->readGpr (r, rval);

          if (err != Dmi::Abstractcs::CMDERR_NONE)
            {
              string regName = fullGprName (r);
              cout << regName << ": " << Utils::padStr (regName, 10)
                   << "Read error: " << err << endl;
              break;
            }

          // R0 should never read back zero,
          if (r == 0)
            {
              if (rval != 0)
                {
                  string regName = fullGprName (r);
                  cout << regName << ": " << Utils::padStr (regName, 10)
                       << "Wrote: 0x" << Utils::hexStr (testvals[i])
                       << ", read back: 0x" << Utils::hexStr (rval) << endl;
                  succeeded = false;
                }
            }
          else
            {
              if (rval != testvals[i])
                {
                  string regName = fullGprName (r);
                  cout << regName << ": " << Utils::padStr (regName, 10)
                       << "Wrote: 0x" << Utils::hexStr (testvals[i])
                       << ", read back: 0x" << Utils::hexStr (rval) << endl;
                  succeeded = false;
                }
            }
        }
    }

  if (succeeded)
    cout << "All GPRs written and read back correctly" << endl;
}

/// \brief Test all the FPRs
///
/// \note There is no option to select a hart, it is presumed selected prior to
///       this.
///
/// \note This will break the debug unit if there is no FPU.
void
TestJtag::testFprs ()
{
  uint32_t regval[32];

  // First report all the FPRs, saving the current value.
  cout << "Reading FPR values:" << endl;
  for (size_t r = 0; r < 32; r++)
    {
      string regName = fullFprName (r);
      Dmi::Abstractcs::CmderrVal err = mDmi->readFpr (r, regval[r]);

      if ((r % REGS_PER_ROW) == 0)
        cout << "  ";

      if (err == Dmi::Abstractcs::CMDERR_NONE)
        cout << regName << Utils::padStr (regName, 10) << " = "
             << Utils::hexStr (regval[r]);
      else
        cout << regName << Utils::padStr (regName, 10) << " = "
             << Utils::hexStr (regval[r]);

      if (((r % REGS_PER_ROW) == (REGS_PER_ROW - 1)) | (r == 31))
        cout << endl;
      else
        cout << "  ";
    }

  // Try to write and read back the registers.
  cout << "Writing FPR values:" << endl;
  bool succeeded = true;
  for (size_t r = 0; r < 32; r++)
    {
      uint32_t testvals[] = { 0xffffffff, 0x00000000, 0xdeadbeef, regval[r] };

      for (size_t i = 0; i < (sizeof (testvals) / sizeof (testvals[0])); i++)
        {
          Dmi::Abstractcs::CmderrVal err
              = mDmi->writeFpr (r, testvals[i]);
          if (err != Dmi::Abstractcs::CMDERR_NONE)
            {
              string regName = fullFprName (r);
              cout << regName << ": " << Utils::padStr (regName, 10)
                   << "Write error: " << err << endl;
              break;
            }

          uint32_t rval;
          err = mDmi->readFpr (r, rval);

          if (err != Dmi::Abstractcs::CMDERR_NONE)
            {
              string regName = fullFprName (r);
              cout << regName << ": " << Utils::padStr (regName, 10)
                   << "Read error: " << err << endl;
              break;
            }

          if (rval != testvals[i])
            {
              string regName = fullFprName (r);
              cout << regName << ": " << Utils::padStr (regName, 10)
                   << "Wrote: 0x" << Utils::hexStr (testvals[i])
                   << ", read back: 0x" << Utils::hexStr (rval) << endl;
              succeeded = false;
            }
        }
    }

  if (succeeded)
    cout << "All FPRs written and read back correctly" << endl;
}

/// \brief Test all the CSRs
///
/// We only try reading each CSR and print out any which are non-zero. Writing
/// CSRs may have unexpected side effects!
///
/// \note There is no option to select a hart, it is presumed selected prior to
///       this.
void
TestJtag::testCsrs ()
{
  // Standard user CSRs
  cout << "Test reading standard user CSRs:" << endl;
  for (auto it = userCsrList.begin (); it != userCsrList.end (); it++)
    if (mDmi->csrType (*it) != Dmi::FP)
      {
        uint32_t val;
        Dmi::Abstractcs::CmderrVal err = mDmi->readCsr (*it, val);

        if (err == Dmi::Abstractcs::CMDERR_NONE)
          cout << "Standard user CSR " << mDmi->csrName (*it) << " (0x"
               << Utils::hexStr (*it, 3) << ") = 0x" << Utils::hexStr (val)
               << endl;
        else
          cout << "Standard user CSR " << mDmi->csrName (*it)
               << ": read error: " << Dmi::Abstractcs::cmderrName (err) << endl;
      }

  cout << endl;

  // Custom user CSRs
  cout << "Test reading custom user CSRs:" << endl;
  for (auto it = customUserCsrList.begin (); it != customUserCsrList.end ();
       it++)
    if (mDmi->csrType (*it) != Dmi::FP)
      {
        uint32_t val;
        Dmi::Abstractcs::CmderrVal err = mDmi->readCsr (*it, val);

        if (err == Dmi::Abstractcs::CMDERR_NONE)
          cout << "Custom user CSR " << mDmi->csrName (*it) << " (0x"
               << Utils::hexStr (*it, 3) << ") = 0x" << Utils::hexStr (val)
               << endl;
        else
          cout << "Custom user CSR " << mDmi->csrName (*it)
               << ": read error: " << Dmi::Abstractcs::cmderrName (err) << endl;
      }
  cout << endl;

  // Standard machine CSRs
  cout << "Test reading standard machine CSRs:" << endl;
  for (auto it = machineCsrList.begin (); it != machineCsrList.end (); it++)
    if (mDmi->csrType (*it) != Dmi::FP)
      {
        uint32_t val;
        Dmi::Abstractcs::CmderrVal err = mDmi->readCsr (*it, val);

        if (err == Dmi::Abstractcs::CMDERR_NONE)
          cout << "Standard machine CSR " << mDmi->csrName (*it) << " (0x"
               << Utils::hexStr (*it, 3) << ") = 0x" << Utils::hexStr (val)
               << endl;
        else
          cout << "Standard machine CSR " << mDmi->csrName (*it)
               << ": read error: " << Dmi::Abstractcs::cmderrName (err) << endl;
      }
  cout << endl;
}

/// \brief Test memory
///
/// Read from memory, then write back and check memory has changed.
///
/// It is generally too slow to access all the memory, so instead we choose
/// three regions
///
/// 1. A block beginning at the start of the region, with an arbitrarily
///    aligned end point.
/// 2. A block in the middle of the region with arbitrarily aligned start and
///    end points.
/// 3. A block ending at the end of the region, with an arbitrarily aligned
///    start point.
///
/// We read the values in the region and save them, the write values in and
/// read them back and verify that the values are unchanged.
///
/// \param[in] region    Textual name of the region
/// \param[in] baseAddr  Base address of the region
/// \param[in] len       Length of the region in octets
/// \param[in] maxBlock  Maximum size of memory block to test
/// \param[in] readOnly  \c true if this is a read only block, \c false
///                      otherwise.
void
TestJtag::testMem (const char *region, uint32_t baseAddr, uint32_t len,
                   size_t maxBlock, bool readOnly)
{
  testMemBlock (region, baseAddr, 1 + Utils::rand (maxBlock), readOnly);

  uint32_t midBlockStart = baseAddr + maxBlock + Utils::rand (len - maxBlock);
  testMemBlock (region, midBlockStart, 1 + Utils::rand (maxBlock), readOnly);

  uint32_t endBlockLen = Utils::rand (maxBlock);
  testMemBlock (region, baseAddr + len - endBlockLen, endBlockLen, readOnly);
}

/// \brief Utility to test a memory block
///
/// - First read the memory into an array
/// - Then check reading and writing (0, 0xff, random)
/// - Then write back original values
///
/// If the memory is read only, we only do the first of these.
///
/// \param[in] region    Textual name of the region
/// \param[in] baseAddr  Base address of the block
/// \param[in] len       Length of the block in octets
/// \param[in] readOnly  \c true if this is a read only block, \c false
///                      otherwise.
void
TestJtag::testMemBlock (const char *region, uint32_t baseAddr, uint32_t len,
                        bool readOnly)
{
  cout << "Testing " << (readOnly ? "read only " : "read/write")
       << "memory region \"" << region << "\": 0x"
       << Utils::hexStr (baseAddr, 8) << " - 0x"
       << Utils::hexStr (baseAddr + len - 1, 8) << endl;

  /// Buffers for various stages
  unique_ptr<uint8_t[]> origBuf (new uint8_t[len]); // Original data
  unique_ptr<uint8_t[]> zeroBuf (new uint8_t[len]); // Buffer of zeros
  unique_ptr<uint8_t[]> onesBuf (new uint8_t[len]); // Buffer of ones
  unique_ptr<uint8_t[]> randBuf (new uint8_t[len]); // Random data

  // Set up the pre-allocated values
  (void)memset (zeroBuf.get (), 0, len);
  (void)memset (onesBuf.get (), 0xff, len);

  for (size_t i = 0; i < len; i++)
    randBuf[i] = static_cast<uint8_t> (Utils::rand (0x100));

  // Read the original values
  Dmi::Sbcs::SberrorVal err = mDmi->readMem (baseAddr, len, origBuf);
  if (err != Dmi::Sbcs::SBERR_NONE)
    {
      cout << "Memory block in " << region << ": original readMem (0x"
           << Utils::hexStr (baseAddr, 8) << ", " << len
           << "): read error: " << Dmi::Sbcs::sberrorName (err) << endl;
      return;
    }

  // If we are read only, then this is all we can do
  if (readOnly)
    return;

  // Write and then read back zeros, then ones, then random
  if (!testWriteRead (region, "zero", baseAddr, len, zeroBuf))
    return;
  if (!testWriteRead (region, "ones", baseAddr, len, onesBuf))
    return;
  if (!testWriteRead (region, "random", baseAddr, len, randBuf))
    return;

  /// Restore original values
  err = mDmi->writeMem (baseAddr, len, origBuf);
  if (err != Dmi::Sbcs::SBERR_NONE)
    {
      cout << "Memory block  in " << region << ": original writeMem (0x"
           << Utils::hexStr (baseAddr, 8) << ", " << len
           << "): write error: " << Dmi::Sbcs::sberrorName (err) << endl;
    }
}

/// \brief Utility to write and read back a memory block
///
/// \param[in] region    Textual name of the region
/// \param[in] testName  Name of the test
/// \param[in] baseAddr  Base address of the block
/// \param[in] len       Length of the block in octets
/// \param[in] origBuf   Buffer of values to write
/// \return \c true on if all reads/write are sucessful, even if there are
///         mismatches, \c false otherwise.
bool
TestJtag::testWriteRead (const char *region, const char *testName,
                         uint32_t baseAddr, uint32_t len,
                         unique_ptr<uint8_t[]> &origBuf)
{
  unique_ptr<uint8_t[]> buf (new uint8_t[len]); // Buffer to read into

  Dmi::Sbcs::SberrorVal err = mDmi->writeMem (baseAddr, len, origBuf);
  if (err != Dmi::Sbcs::SBERR_NONE)
    {
      cout << "Memory block  in " << region << ": " << testName
           << " writeMem (0x" << Utils::hexStr (baseAddr, 8) << ", " << len
           << "): write error: " << Dmi::Sbcs::sberrorName (err) << endl;
      return false;
    }

  err = mDmi->readMem (baseAddr, len, buf);
  if (err != Dmi::Sbcs::SBERR_NONE)
    {
      cout << "Memory block  in " << region << ": " << testName
           << " readMem (0x" << Utils::hexStr (baseAddr, 8) << ", " << len
           << "): read error: " << Dmi::Sbcs::sberrorName (err) << endl;
      return false;
    }

  for (size_t i = 0; i < len; i++)
    if (buf[i] != origBuf[i])
      cout << "- " << region << ", test " << testName << ": at 0x"
           << Utils::hexStr (baseAddr + i, 8) << ": wrote 0x"
           << Utils::hexStr (origBuf[i], 2) << ", read back 0x"
           << Utils::hexStr (buf[i], 2) << endl;

  return true;
}

/// \brief Utility to convert GPR number to ABI register name.
///
/// \param[in] regno  The register number
/// \return The register name;
const char *
TestJtag::gprAbiName (size_t regno)
{
  static const char *regName[] = {
    "zero", "ra", "sp", "gp", "tp",  "t0",  "t1", "t2", "s0/fp", "s1", "a0",
    "a1",   "a2", "a2", "a4", "a5",  "a6",  "a7", "s2", "s3",    "s4", "s5",
    "s6",   "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5",    "t6",
  };
  size_t numRegno = sizeof (regName) / sizeof (regName[0]);

  return (regno < numRegno) ? regName[regno] : "(unknown)";
}

/// \brief Utility to convert GPR number to full register name.
///
/// The full name is the ABI name with the raw name in parentheses.
///
/// \param[in] regno  The register number
/// \return The register name;
string
TestJtag::fullGprName (size_t regno)
{
  sOss.str ("");
  sOss << gprAbiName (regno) << " (x" << regno << ")";
  return sOss.str ();
}

/// \brief Utility to convert FPR number to ABI register name.
///
/// \param[in] regno  The register number
/// \return The register name;
const char *
TestJtag::fprAbiName (size_t regno)
{
  static const char *regName[] = {
    "ft0", "ft1", "ft2",  "ft3",  "ft4", "ft5", "ft6",  "ft6",
    "fs0", "fs1", "fa0",  "fa1",  "fa2", "fa3", "fa4",  "fa5",
    "fa6", "fa7", "fs2",  "fs3",  "fs4", "fs5", "fs6",  "fs7",
    "fs8", "fs9", "fs10", "fs11", "ft8", "ft9", "ft10", "ft11",
  };
  size_t numRegno = sizeof (regName) / sizeof (regName[0]);

  return (regno < numRegno) ? regName[regno] : "(unknown)";
}

/// \brief Utility to convert FPR number to full register name.
///
/// The full name is the ABI name with the raw name in parentheses.
///
/// \param[in] regno  The register number
/// \return The register name;
string
TestJtag::fullFprName (size_t regno)
{
  sOss.str ("");
  sOss << fprAbiName (regno) << " (f" << regno << ")";
  return sOss.str ();
}

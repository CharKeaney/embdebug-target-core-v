// Definition of the generic Debug Module Interface
//
// This file is part of the Embecosm Debug Server target for CORE-V MCU
//
// Copyright (C) 2021 Embecosm Limited
// SPDX-License-Identifier: Apache-2.0

#include <iomanip>
#include <iostream>
#include <sstream>

#include "Dmi.h"

using std::cerr;
using std::cout;
using std::dec;
using std::endl;
using std::hex;
using std::ostream;
using std::setfill;
using std::setw;
using std::size_t;
using std::unique_ptr;

/// \brief Constructor for the DMI
///
/// The DTM has to be constructed locally, but is then passed to us, and we
/// take ownership.  It is a unique_ptr, so the call to the constructor must
/// explicitly move the pointer.
///
/// We create local instances of all the interesting registers
///
/// \param[in] dtm_  The Debug Transport Module we will use.
Dmi::Dmi (unique_ptr<IDtm> dtm_) : mDtm (std::move (dtm_))
{
  mData.reset (new Data (mDtm));
  mDmcontrol.reset (new Dmcontrol (mDtm));
  mDmstatus.reset (new Dmstatus (mDtm));
  mHartinfo.reset (new Hartinfo (mDtm));
  mHaltsum.reset (new Haltsum (mDtm));
  mHawindowsel.reset (new Hawindowsel (mDtm));
  mHawindow.reset (new Hawindow (mDtm));
  mAbstractcs.reset (new Abstractcs (mDtm));
  mCommand.reset (new Command (mDtm));
  mAbstractauto.reset (new Abstractauto (mDtm));
  mConfstrptr.reset (new Confstrptr (mDtm));
  mNextdm.reset (new Nextdm (mDtm));
  mProgbuf.reset (new Progbuf (mDtm));
  mAuthdata.reset (new Authdata (mDtm));
  mSbaddress.reset (new Sbaddress (mDtm));
  mSbcs.reset (new Sbcs (mDtm));
  mSbdata.reset (new Sbdata (mDtm));
}

/// \brief Reset the underlying DTM.
void
Dmi::dtmReset ()
{
  mDtm->reset ();
}

/// \brief Get the \p data register.
///
/// \return  An instance of class Data:: representing the set of \c data
///          registers.
std::unique_ptr<Dmi::Data> &
Dmi::data ()
{
  return mData;
}

/// \brief Get the \p dmcontrol register.
///
/// \return  An instance of class Dmcontrol:: representing the \c dmcontrol
///          register.
std::unique_ptr<Dmi::Dmcontrol> &
Dmi::dmcontrol ()
{
  return mDmcontrol;
}

/// \brief Get the \p dmstatus register.
///
/// \return  An instance of class Dmstatus:: representing the \c dmstatus
///          register.
std::unique_ptr<Dmi::Dmstatus> &
Dmi::dmstatus ()
{
  return mDmstatus;
}

/// \brief Get the \p hartinfo register.
///
/// \return  An instance of class Hartinfo:: representing the \c hartinfo
///          register.
std::unique_ptr<Dmi::Hartinfo> &
Dmi::hartinfo ()
{
  return mHartinfo;
}

/// \brief Get the \p haltsum register.
///
/// \return  An instance of class Haltsum:: representing the set of \c haltsum
///          registers.
std::unique_ptr<Dmi::Haltsum> &
Dmi::haltsum ()
{
  return mHaltsum;
}

/// \brief Get the \p hawindowsel register.
///
/// \return  An instance of class Hawindowsel:: representing the \c hawindowsel
///          register.
std::unique_ptr<Dmi::Hawindowsel> &
Dmi::hawindowsel ()
{
  return mHawindowsel;
}

/// \brief Get the \p hawindow register.
///
/// \return  An instance of class Hawindow:: representing the \c hawindow
///          register.
std::unique_ptr<Dmi::Hawindow> &
Dmi::hawindow ()
{
  return mHawindow;
}

/// \brief Get the \p abstractcs register.
///
/// \return  An instance of class Abstractcs:: representing the \c abstractcs
///          register.
std::unique_ptr<Dmi::Abstractcs> &
Dmi::abstractcs ()
{
  return mAbstractcs;
}

/// \brief Get the \p command register.
///
/// \return  An instance of class Command:: representing the \c command
///          register.
std::unique_ptr<Dmi::Command> &
Dmi::command ()
{
  return mCommand;
}

/// \brief Get the \p abstractauto register.
///
/// \return  An instance of class Abstractauto:: representing the
///          \c abstractauto register.
std::unique_ptr<Dmi::Abstractauto> &
Dmi::abstractauto ()
{
  return mAbstractauto;
}

/// \brief Get the \p constrptr register.
///
/// \return  An instance of class Constrptr:: representing the set of \c
///          confstrptr registers.
std::unique_ptr<Dmi::Confstrptr> &
Dmi::confstrptr ()
{
  return mConfstrptr;
}

/// \brief Get the \p nextdm register.
///
/// \return  An instance of class Nextdm:: representing the \c nextdm
///          register.
std::unique_ptr<Dmi::Nextdm> &
Dmi::nextdm ()
{
  return mNextdm;
}

/// \brief Get the \p progbuf register.
///
/// \return  An instance of class Progbuf:: representing the set of \c progbuf
///          registers.
std::unique_ptr<Dmi::Progbuf> &
Dmi::progbuf ()
{
  return mProgbuf;
}

/// \brief Get the \p authdata register.
///
/// \return  An instance of class Authdata:: representing the \c authdata
///          register.
std::unique_ptr<Dmi::Authdata> &
Dmi::authdata ()
{
  return mAuthdata;
}

/// \brief Get the \p sbaddress register.
///
/// \return  An instance of class Sbaddress:: representing the set of
///          \c sbaddress registers.
std::unique_ptr<Dmi::Sbaddress> &
Dmi::sbaddress ()
{
  return mSbaddress;
}

/// \brief Get the \p sbcs register.
///
/// \return  An instance of class Sbcs:: representing the \c sbcs
///          register.
std::unique_ptr<Dmi::Sbcs> &
Dmi::sbcs ()
{
  return mSbcs;
}

/// \brief Get the \p sbdata register.
///
/// \return  An instance of class Sbdata:: representing the set of
///          \c sbdata registers.
std::unique_ptr<Dmi::Sbdata> &
Dmi::sbdata ()
{
  return mSbdata;
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Data class                                                         *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Data class
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Data::Data (unique_ptr<IDtm> &dtm_) : mDtm (dtm_)
{
  for (size_t i = 0; i < NUM_REGS; i++)
    mDataReg[i] = 0x0;
}

/// \brief Read the value of the specified abstract \c data register.
///
/// The register is refreshed via the DTM.
///
/// \param[in] n  Index of the \c data register to read.
void
Dmi::Data::read (const size_t n)
{
  if (n < NUM_REGS)
    mDataReg[n] = mDtm->dmiRead (DMI_ADDR[n]);
  else
    cerr << "Warning: reading data[" << n << "] invalid: ignored." << endl;
}

/// \brief Set the specified abstract \c data register to its reset value.
void
Dmi::Data::reset (const size_t n)
{
  if (n < NUM_REGS)
    mDataReg[n] = RESET_VALUE;
  else
    cerr << "Warning: reseting data[" << n << "] invalid: ignored." << endl;
}

/// \brief Write the value of the specified abstract \c data register.
///
/// The register is refreshed via the DTM, and we save the value read back.
void
Dmi::Data::write (const size_t n)
{
  if (n < NUM_REGS)
    mDataReg[n] = mDtm->dmiWrite (DMI_ADDR[n], mDataReg[n]);
  else
    cerr << "Warning: writing data[" << n << "] invalid: ignored." << endl;
}

/// \brief Must define as well as declare our private constexpr before using.
constexpr uint64_t Dmi::Data::DMI_ADDR[Dmi::Data::NUM_REGS];

/// Get the value the specified \c data register.
///
/// \param[in] n  Index of the \c data register to get.
/// \return  The value in the specified \c data register.
uint32_t
Dmi::Data::data (const size_t n) const
{
  if (n < NUM_REGS)
    return mDataReg[n];
  else
    {
      cerr << "Warning: getting data[" << n << "] invalid: zero returned."
           << endl;
      return 0;
    }
}

/// Set the value the specified \c data register.
///
/// \param[in] n         Index of the \c data register to get.
/// \param[in] dataValy  The value to be set in the specified \c data register.
void
Dmi::Data::data (const size_t n, const uint32_t dataVal)
{
  if (n < NUM_REGS)
    mDataReg[n] = dataVal;
  else
    cerr << "Warning: setting data[" << n << "] invalid: ignored." << endl;
}

/// \brief Output operator for the Dmi::Data class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Data> &p)
{
  std::ostringstream oss;
  oss << "[" << hex << setw (8) << setfill ('0');

  for (size_t i = 0; i < p->NUM_REGS; i++)
    {
      oss << p->mDataReg[i];
      if (i != (p->NUM_REGS - 1))
        oss << ", ";
    }

  oss << "]";
  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Dmcontrol class                                                    *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Dmcontrol class.
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Dmcontrol::Dmcontrol (unique_ptr<IDtm> &dtm_)
    : mPrettyPrint (false), mDtm (dtm_),
      mDmcontrolReg (Dmi::Dmcontrol::RESET_VALUE)
{
}

/// \brief Read the value of the \c dmcontrol register.
///
/// The register is refreshed via the DTM.
void
Dmi::Dmcontrol::read ()
{
  mDmcontrolReg = mDtm->dmiRead (DMI_ADDR);
}

/// \brief Set the \c dmcontrol register to its reset value.
void
Dmi::Dmcontrol::reset ()
{
  mDmcontrolReg = RESET_VALUE;
}

/// \brief Write the value of the \c dmcontrol register.
///
/// The register is refreshed via the DTM, and we save the value read back.
void
Dmi::Dmcontrol::write ()
{
  mDmcontrolReg = mDtm->dmiWrite (DMI_ADDR, mDmcontrolReg);
}

/// \brief Control whether to pretty print the \c dmcontrol register.
///
/// @param[in] flag  If \c true, subsequent stream output will generate a list
///                  of field values, if false a simple hex value.
void
Dmi::Dmcontrol::prettyPrint (const bool flag)
{
  mPrettyPrint = flag;
}

/// \brief Set the \c haltreq bit in \c dmcontrol.
///
/// \note Value set will apply to the \em new value of \c hartsel.
///
/// \param[in] flag  \c true sets the halt request bit for all currently
///                  selected harts, so that running hards will halt whenever
///                  their halt request bit is set.\c false clears the halt
///                  request bit,
void
Dmi::Dmcontrol::haltreq (const bool flag)
{
  if (flag)
    mDmcontrolReg |= HALTREQ_MASK;
  else
    mDmcontrolReg &= ~HALTREQ_MASK;
}

/// \brief Set the \c resumereq bit in \c dmcontrol to 1.
void
Dmi::Dmcontrol::resumereq ()
{
  mDmcontrolReg |= RESUMEREQ_MASK;
}

/// \brief Get the \c hartreset bit in \c dmcontrol (NOT IMPLEMENTED).
///
/// \warning Not implemented for the CORE-V MCU debug unit. Always returns
///          \c false.
///
/// \returns Always returns \c false.
bool
Dmi::Dmcontrol::hartreset () const
{
  return false;
}

/// \brief Set the \c hartreset bit in \c dmcontrol (NOT IMPLEMENTED).
///
/// \warning Not implemented for the CORE-V MCU debug unit. Ignored with a
///          warning.
///
/// \param[in] flag  Value ignored.
void
Dmi::Dmcontrol::hartreset (const bool flag __attribute ((unused)))
{
  cerr << "Warning: Setting dmcontrol:hartreset not supported: ignored."
       << endl;
}

/// \brief Set the \c ackhavereset bit in \c dmcontrol to 1.
void
Dmi::Dmcontrol::ackhavereset ()
{
  mDmcontrolReg |= ACKHAVERESET_MASK;
}

/// \brief Get the \c hasel bit in \c dmcontrol (NOT IMPLEMENTED).
///
/// \warning Not implemented for the CORE-V MCU debug unit. Always returns
///          \c false (only ever one selected hart).
///
/// \return Always returns \c false.
bool
Dmi::Dmcontrol::hasel () const
{
  return false;
}

/// \brief Set the \c hasel bit in \c dmcontrol (NOT IMPLEMENTED).
///
/// \warning Not implemented for the CORE-V MCU debug unit. Ignored with a
///          warning.
///
/// \param[in] flag  Value ignored.
void
Dmi::Dmcontrol::hasel (const bool flag __attribute ((unused)))
{
  cerr << "Warning: Setting dmcontrol:hasel not supported: ignored." << endl;
}

/// \brief Get the \c hartsello and \c hartselhi bits of \c dmcontrol
///
/// Computes \c hartsel as \c hartselhi << 10 | \c hartsello.
///
/// \return The value of \c hartsel
uint32_t
Dmi::Dmcontrol::hartsel () const
{
  uint32_t hartsello = (mDmcontrolReg & (HARTSELLO_MASK)) >> HARTSELLO_OFFSET;
  uint32_t hartselhi = (mDmcontrolReg & (HARTSELHI_MASK)) >> HARTSELHI_OFFSET;
  return (hartselhi << HARTSELLO_SIZE) | hartsello;
}

/// \brief Set the \c hartsello and \c hartselhi bits of \c dmcontrol
///
/// Sets \c hartslehi to \p (hartselVal >> 10) & 0x3ff
/// Sets \c hartsello to \p hartselVal & 0x3ff
///
/// \param[in] hartselVal  The value of \c hartsel to set.
void
Dmi::Dmcontrol::hartsel (const uint32_t hartselVal)
{
  if (hartselVal >= (1 << (HARTSELLO_SIZE + HARTSELHI_SIZE)))
    cerr << "Warning: requested value of hartsel, " << hartselVal
         << ", exceeds the maximum permitted value: higher bits ignored."
         << endl;

  uint32_t hartsello = (hartselVal << HARTSELLO_OFFSET) & HARTSELLO_MASK;
  uint32_t hartselhi
      = ((hartselVal >> HARTSELLO_SIZE) << HARTSELHI_OFFSET) & HARTSELHI_MASK;
  mDmcontrolReg &= ~(HARTSELLO_MASK | HARTSELHI_MASK);
  mDmcontrolReg |= hartselhi | hartsello;
}

/// \brief Set the \c hartsello and \c hartselhi bits of \c dmcontrol to their
///        maximum possible value.
///
/// Sets \c hartslehi to 0xf3ff
/// Sets \c hartsello to 0xf3ff
void
Dmi::Dmcontrol::hartselMax ()
{
  mDmcontrolReg |= HARTSELHI_MASK | HARTSELLO_MASK;
}
/// \brief Set the \c setresethaltreq bit in \c dmcontrol to 1 (NOT
///        IMPLEMENTED).
///
/// \warning Not implemented for the CORE-V MCU debug unit. Ignored with a
///          warning.
void
Dmi::Dmcontrol::setresethaltreq ()
{
  cerr << "Warning: Setting dmcontrol:setresethaltreq not supported: ignored."
       << endl;
}

/// \brief Set the \c clrresethaltreq bit in \c dmcontrol to 1 (NOT
///        IMPLEMENTED).
///
/// \warning Not implemented for the CORE-V MCU debug unit. Ignored with a
///          warning.
void
Dmi::Dmcontrol::clrresethaltreq ()
{
  cerr << "Warning: Setting dmcontrol:clrresethaltreq not supported: ignored."
       << endl;
}

/// \brief Get the \c ndmreset bit in \c dmcontrol
///
/// return \c true if the \c ndmreset bit is set and \c false otherwise.
bool
Dmi::Dmcontrol::ndmreset () const
{
  return (mDmcontrolReg & NDMRESET_MASK) != 0;
}

/// \brief Set the \c ndmreset bit in \c dmcontrol
///
/// \param[in] flag  If \c true sets the \c ndmreset bit, otherwise clears the
///                  \c ndmreset bit.
void
Dmi::Dmcontrol::ndmreset (const bool flag)
{
  if (flag)
    mDmcontrolReg |= NDMRESET_MASK;
  else
    mDmcontrolReg &= ~NDMRESET_MASK;
}

/// \brief Get the \c dmactive bit in \c dmcontrol
///
/// return \c true if the \c dmactive bit is set and \c false otherwise.
bool
Dmi::Dmcontrol::dmactive () const
{
  return (mDmcontrolReg & DMACTIVE_MASK) != 0;
}

/// \brief Set the \c dmactive bit in \c dmcontrol
///
/// \param[in] flag  If \c true sets the \c dmactive bit, otherwise clears the
///                  \c dmactive bit.
void
Dmi::Dmcontrol::dmactive (const bool flag)
{
  if (flag)
    mDmcontrolReg |= DMACTIVE_MASK;
  else
    mDmcontrolReg &= ~DMACTIVE_MASK;
}

/// \brief Output operator for the Dmi::Dmcontrol class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Dmcontrol> &p)
{
  std::ostringstream oss;

  if (p->mPrettyPrint)
    oss << "[ haltreq = "
        << ((p->mDmcontrolReg & p->HALTREQ_MASK) != 0 ? "true" : "false")
        << ", resumereq = "
        << ((p->mDmcontrolReg & p->RESUMEREQ_MASK) != 0 ? "true" : "false")
        << ", hartreset = " << (p->hartreset () ? "true" : "false")
        << ", ackhavereset = "
        << ((p->mDmcontrolReg & p->ACKHAVERESET_MASK) != 0 ? "true" : "false")
        << ", hasel = " << (p->hasel () ? "true" : "false") << ", hartsel = 0x"
        << hex << setw (5) << setfill ('0') << p->hartsel ()
        << ", setresethaltreq = "
        << ((p->mDmcontrolReg & p->SETRESETHALTREQ_MASK) != 0 ? "true"
                                                              : "false")
        << ", clrresethaltreq = "
        << ((p->mDmcontrolReg & p->CLRRESETHALTREQ_MASK) != 0 ? "true"
                                                              : "false")
        << ", ndmreset = " << (p->ndmreset () ? "true" : "false")
        << ", dmactive = " << (p->dmactive () ? "true" : "false") << " ]";
  else
    oss << hex << setw (8) << setfill ('0') << p->mDmcontrolReg;

  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Dmstatus class                                                     *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Dmstatus class
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Dmstatus::Dmstatus (unique_ptr<IDtm> &dtm_)
    : mPrettyPrint (false), mDtm (dtm_), mDmstatusReg (0)
{
}

/// \brief Read the value of the \c dmstatus register.
///
/// The register is refreshed via the DTM.
void
Dmi::Dmstatus::read ()
{
  mDmstatusReg = mDtm->dmiRead (DMI_ADDR);
}

/// \brief Control whether to pretty print the \c dmstatus register.
///
/// @param[in] flag  If \c true, subsequent stream output will generate a list
///                  of field values, if false a simple hex value.
void
Dmi::Dmstatus::prettyPrint (const bool flag)
{
  mPrettyPrint = flag;
}

/// Get the \c impebreak field of the \c dmstatus register.
///
/// \return \c true if the \c impebreak field of \c dmstatus is set, \c false
///         otherwise.
bool
Dmi::Dmstatus::impebreak () const
{
  return (mDmstatusReg & IMPEBREAK_MASK) != 0;
}

/// Get the \c havereset fields of the \c dmstatus register.
///
/// \note we do not distinguish between "all" or "any" versions of the flag.
///
/// \return \c true if either of the \c allhavereset of \c anyhavereset
///         fields of \c dmstatus is set, \c false otherwise.
bool
Dmi::Dmstatus::havereset () const
{
  return (mDmstatusReg & (ALLHAVERESET_MASK | ANYHAVERESET_MASK)) != 0;
}

/// Get the \c resumeack fields of the \c dmstatus register.
///
/// \note we do not distinguish between "all" or "any" versions of the flag.
///
/// \return \c true if either of the \c allresumeack of \c anyresumeack
///         fields of \c dmstatus is set, \c false otherwise.
bool
Dmi::Dmstatus::resumeack () const
{
  return (mDmstatusReg & (ALLRESUMEACK_MASK | ANYRESUMEACK_MASK)) != 0;
}

/// Get the \c nonexistent fields of the \c dmstatus register.
///
/// \note we do not distinguish between "all" or "any" versions of the flag.
///
/// \return \c true if either of the \c allnonexistent of \c anynonexistent
///         fields of \c dmstatus is set, \c false otherwise.
bool
Dmi::Dmstatus::nonexistent () const
{
  return (mDmstatusReg & (ALLNONEXISTENT_MASK | ANYNONEXISTENT_MASK)) != 0;
}

/// Get the \c unavail fields of the \c dmstatus register.
///
/// \note we do not distinguish between "all" or "any" versions of the flag.
///
/// \return \c true if either of the \c allunavail of \c anyunavail
///         fields of \c dmstatus is set, \c false otherwise.
bool
Dmi::Dmstatus::unavail () const
{
  return (mDmstatusReg & (ALLUNAVAIL_MASK | ANYUNAVAIL_MASK)) != 0;
}

/// Get the \c running fields of the \c dmstatus register.
///
/// \note we do not distinguish between "all" or "any" versions of the flag.
///
/// \return \c true if either of the \c allrunning of \c anyrunning
///         fields of \c dmstatus is set, \c false otherwise.
bool
Dmi::Dmstatus::running () const
{
  return (mDmstatusReg & (ALLRUNNING_MASK | ANYRUNNING_MASK)) != 0;
}

/// Get the \c halted fields of the \c dmstatus register.
///
/// \note we do not distinguish between "all" or "any" versions of the flag.
///
/// \return \c true if either of the \c allhalted of \c anyhalted
///         fields of \c dmstatus is set, \c false otherwise.
bool
Dmi::Dmstatus::halted () const
{
  return (mDmstatusReg & (ALLHALTED_MASK | ANYHALTED_MASK)) != 0;
}

/// Get the \c authenticated field of the \c dmstatus register.
///
/// \return \c true if the \c authenticated field of \c dmstatus is set,
///         \c false otherwise.
bool
Dmi::Dmstatus::authenticated () const
{
  return (mDmstatusReg & AUTHENTICATED_MASK) != 0;
}

/// Get the \c authbusy field of the \c dmstatus register.
///
/// \return \c true if the \c authbusy field of \c dmstatus is set, \c false
///         otherwise.
bool
Dmi::Dmstatus::authbusy () const
{
  return (mDmstatusReg & AUTHBUSY_MASK) != 0;
}

/// Get the \c hasresethaltreq field of the \c dmstatus register.
///
/// \return \c true if the \c hasresethaltreq field of \c dmstatus is set,
///         \c false otherwise.
bool
Dmi::Dmstatus::hasresethaltreq () const
{
  return (mDmstatusReg & HASRESETHALTREQ_MASK) != 0;
}

/// Get the \c confstrptrvalid field of the \c dmstatus register.
///
/// \return \c true if the \c confstrptrvalid field of \c dmstatus is set,
///         \c false otherwise.
bool
Dmi::Dmstatus::confstrptrvalid () const
{
  return (mDmstatusReg & CONFSTRPTRVALID_MASK) != 0;
}

/// Get the \c version field of the \c dmstatus register.
///
/// \return the value in the \c version field of \c dmstatus.
uint8_t
Dmi::Dmstatus::version () const
{
  return (mDmstatusReg & VERSION_MASK) >> VERSION_OFFSET;
}

/// \brief Output operator for the Dmi::Dmstatus class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Dmstatus> &p)
{
  std::ostringstream oss;

  if (p->mPrettyPrint)
    oss << "[ impebreak = " << (p->impebreak () ? "true" : "false")
        << ", havereset = " << (p->havereset () ? "true" : "false")
        << ", resumeack = " << (p->resumeack () ? "true" : "false")
        << ", nonexistent = " << (p->nonexistent () ? "true" : "false")
        << ", unavail = " << (p->unavail () ? "true" : "false")
        << ", running = " << (p->running () ? "true" : "false")
        << ", halted = " << (p->halted () ? "true" : "false")
        << ", authenticated = " << (p->authenticated () ? "true" : "false")
        << ", authbusy = " << (p->authbusy () ? "true" : "false")
        << ", hasresethaltreq = " << (p->hasresethaltreq () ? "true" : "false")
        << ", confstrptrvalid = " << (p->confstrptrvalid () ? "true" : "false")
        << ", version = " << static_cast<uint16_t> (p->version ()) << " ]";
  else
    oss << hex << setw (8) << setfill ('0') << p->mDmstatusReg;

  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Hartinfo class                                                     *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Hartinfo class
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Hartinfo::Hartinfo (unique_ptr<IDtm> &dtm_)
    : mPrettyPrint (false), mDtm (dtm_), mHartinfoReg (0)
{
}

/// \brief Read the value of the \c hartinfo register.
///
/// The register is refreshed via the DTM.
void
Dmi::Hartinfo::read ()
{
  mHartinfoReg = mDtm->dmiRead (DMI_ADDR);
}

/// \brief Control whether to pretty print the \c hartinfo register.
///
/// @param[in] flag  If \c true, subsequent stream output will generate a list
///                  of field values, if false a simple hex value.
void
Dmi::Hartinfo::prettyPrint (const bool flag)
{
  mPrettyPrint = flag;
}

/// Get the \c nscratch field of the \c hartinfo register.
///
/// \return  The value in the \c nscratch field of the \c hartinfo register
uint8_t
Dmi::Hartinfo::nscratch () const
{
  return (mHartinfoReg & NSCRATCH_MASK) >> NSCRATCH_OFFSET;
}

/// Get the \c dataaccess field of the \c hartinfo register.
///
/// \return  \c true if \c dataaccess field of \c hartinfo is set, \c false
///          otherwise.
bool
Dmi::Hartinfo::dataaccess () const
{
  return (mHartinfoReg & DATAACCESS_MASK) != 0;
}

/// Get the \c datasize field of the \c hartinfo register.
///
/// \return  The value in the \c datasize field of the \c hartinfo register
uint8_t
Dmi::Hartinfo::datasize () const
{
  return (mHartinfoReg & DATASIZE_MASK) >> DATASIZE_OFFSET;
}

/// Get the \c dataaddr field of the \c hartinfo register.
///
/// \return  The value in the \c dataaddr field of the \c hartinfo register
uint16_t
Dmi::Hartinfo::dataaddr () const
{
  return (mHartinfoReg & DATAADDR_MASK) >> DATAADDR_OFFSET;
}

/// \brief Output operator for the Dmi::Hartinfo class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Hartinfo> &p)
{
  std::ostringstream oss;

  if (p->mPrettyPrint)
    oss << "[ nscratch = " << static_cast<uint16_t> (p->nscratch ())
        << ", dataaccess = " << (p->dataaccess () ? "true" : "false")
        << ", datasize = " << static_cast<uint16_t> (p->datasize ())
        << ", dataaddr = 0x" << hex << setw (3) << setfill ('0')
        << p->dataaddr () << " ]";
  else
    oss << hex << setw (8) << setfill ('0') << p->mHartinfoReg;

  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Haltsum class                                                      *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Haltsum class
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Haltsum::Haltsum (unique_ptr<IDtm> &dtm_) : mDtm (dtm_)
{
  for (size_t i = 0; i < NUM_REGS; i++)
    mHaltsumReg[i] = 0x0;
}

/// \brief Read the value of the specified \c haltsum register.
///
/// The register is refreshed via the DTM.
///
/// \param[in] n  Index of the \c haltsum register to read.
void
Dmi::Haltsum::read (const size_t n)
{
  if (n < NUM_REGS)
    mHaltsumReg[n] = mDtm->dmiRead (DMI_ADDR[n]);
  else
    cerr << "Warning: reading haltsum[" << n << "] invalid: ignored." << endl;
}

/// \brief Must define as well as declare our private constexpr before using.
constexpr uint64_t Dmi::Haltsum::DMI_ADDR[Dmi::Haltsum::NUM_REGS];

/// Get the value the specified \c haltsum register.
///
/// \param[in] n  Index of the \c haltsum register to get.
/// \return  The value in the specified \c haltsum register.
uint32_t
Dmi::Haltsum::haltsum (const size_t n) const
{
  if (n < NUM_REGS)
    return mHaltsumReg[n];
  else
    {
      cerr << "Warning: getting haltsum[" << n << "] invalid: zero returned."
           << endl;
      return 0;
    }
}

/// \brief Output operator for the Dmi::Haltsum class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Haltsum> &p)
{
  std::ostringstream oss;
  oss << "[" << hex << setw (8) << setfill ('0');

  for (size_t i = 0; i < p->NUM_REGS; i++)
    {
      oss << p->mHaltsumReg[i];
      if (i != (p->NUM_REGS - 1))
        oss << ", ";
    }

  oss << "]";
  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Hawindowsel class                                                  *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Hawindowsel class.
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Hawindowsel::Hawindowsel (unique_ptr<IDtm> &dtm_)
    : mDtm (dtm_), mHawindowselReg (Dmi::Hawindowsel::RESET_VALUE)
{
}

/// \brief Read the value of the \c hawindowsel register.
///
/// The register is refreshed via the DTM.
void
Dmi::Hawindowsel::read ()
{
  mHawindowselReg = mDtm->dmiRead (DMI_ADDR);
}

/// \brief Set the \c hawindowsel register to its reset value.
void
Dmi::Hawindowsel::reset ()
{
  mHawindowselReg = RESET_VALUE;
}

/// \brief Write the value of the \c hawindowsel register.
///
/// The register is refreshed via the DTM, and we save the value read back.
void
Dmi::Hawindowsel::write ()
{
  mHawindowselReg = mDtm->dmiWrite (DMI_ADDR, mHawindowselReg);
}

/// \brief Get the \c hawindowsel bits of \c hawindowsel
///
/// \return The value of \c hawindowsel
uint16_t
Dmi::Hawindowsel::hawindowsel () const
{
  return static_cast<uint16_t> ((mHawindowselReg & HAWINDOWSEL_MASK)
                                >> HAWINDOWSEL_OFFSET);
}

/// \brief Set the \c hawindowsel bits of \c hawindowsel
///
/// \param[in] hawindowselVal  The value of \c hawindowsel to set.
void
Dmi::Hawindowsel::hawindowsel (const uint16_t hawindowselVal)
{
  if (hawindowselVal >= (1 << HAWINDOWSEL_SIZE))
    cerr << "Warning: requested value of hawindowsel, " << hawindowselVal
         << ", exceeds the maximum permitted value: higher bits ignored."
         << endl;

  mHawindowselReg &= ~HAWINDOWSEL_MASK;
  mHawindowselReg |= (hawindowselVal << HAWINDOWSEL_OFFSET) & HAWINDOWSEL_MASK;
}

/// \brief Output operator for the Dmi::Hawindowsel class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Hawindowsel> &p)
{
  std::ostringstream oss;
  oss << hex << setw (8) << setfill ('0') << p->mHawindowselReg;
  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Hawindow class                                                     *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Hawindow class.
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Hawindow::Hawindow (unique_ptr<IDtm> &dtm_)
    : mDtm (dtm_), mHawindowReg (Dmi::Hawindow::RESET_VALUE)
{
}

/// \brief Read the value of the \c hawindow register.
///
/// The register is refreshed via the DTM.
void
Dmi::Hawindow::read ()
{
  mHawindowReg = mDtm->dmiRead (DMI_ADDR);
}

/// \brief Set the \c hawindow register to its reset value.
void
Dmi::Hawindow::reset ()
{
  mHawindowReg = RESET_VALUE;
}

/// \brief Write the value of the \c hawindow register.
///
/// The register is refreshed via the DTM, and we save the value read back.
void
Dmi::Hawindow::write ()
{
  mHawindowReg = mDtm->dmiWrite (DMI_ADDR, mHawindowReg);
}

/// \brief Get the value of \c hawindow
///
/// \return The value of \c hawindow
uint32_t
Dmi::Hawindow::hawindow () const
{
  return mHawindowReg;
}

/// \brief Set the value of \c hawindow
///
/// \param[in] hawindowVal  The value of \c hawindow to set.
void
Dmi::Hawindow::hawindow (const uint32_t hawindowVal)
{
  mHawindowReg = hawindowVal;
}

/// \brief Output operator for the Dmi::Hawindow class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Hawindow> &p)
{
  std::ostringstream oss;
  oss << hex << setw (8) << setfill ('0') << p->mHawindowReg;
  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Abstractcs class                                                   *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Abstractcs class.
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Abstractcs::Abstractcs (unique_ptr<IDtm> &dtm_)
    : mPrettyPrint (false), mDtm (dtm_),
      mAbstractcsReg (Dmi::Abstractcs::RESET_VALUE)
{
}

/// \brief Read the value of the \c abstractcs register.
///
/// The register is refreshed via the DTM.
void
Dmi::Abstractcs::read ()
{
  mAbstractcsReg = mDtm->dmiRead (DMI_ADDR);
}

/// \brief Set the \c abstractcs register to its reset value.
void
Dmi::Abstractcs::reset ()
{
  mAbstractcsReg = RESET_VALUE;
}

/// \brief Write the value of the \c abstractcs register.
///
/// The register is refreshed via the DTM, and we save the value read back.
void
Dmi::Abstractcs::write ()
{
  mAbstractcsReg = mDtm->dmiWrite (DMI_ADDR, mAbstractcsReg);
}

/// \brief Control whether to pretty print the \c abstractcs register.
///
/// @param[in] flag  If \c true, subsequent stream output will generate a list
///                  of field values, if false a simple hex value.
void
Dmi::Abstractcs::prettyPrint (const bool flag)
{
  mPrettyPrint = flag;
}

/// \brief Get the \c progbufsize bits of \c abstractcs
///
/// \return The value of \c progbufsize
uint8_t
Dmi::Abstractcs::progbufsize () const
{
  return static_cast<uint8_t> ((mAbstractcsReg & PROGBUFSIZE_MASK)
                               >> PROGBUFSIZE_OFFSET);
}

/// \brief Get the \c busy bit of \c abstractcs
///
/// \return \c true if the \c busy bit is set, \c false otherwise.
bool
Dmi::Abstractcs::busy () const
{
  return (mAbstractcsReg & PROGBUFSIZE_MASK) != 0;
}

/// \brief Get the \c cmderr bits of \c abstractcs
///
/// \return The value of \c cmderr
uint8_t
Dmi::Abstractcs::cmderr () const
{
  return static_cast<uint8_t> ((mAbstractcsReg & CMDERR_MASK) >> CMDERR_OFFSET);
}

/// \brief Clear the \c cmderr bits of \c abstractcs
///
/// This means setting all the bits to 1, prior to a write.
void
Dmi::Abstractcs::cmderrClear ()
{
  mAbstractcsReg |= CMDERR_MASK;
}

/// \brief Get the \c datacount bits of \c abstractcs
///
/// \return The value of \c datacount
uint8_t
Dmi::Abstractcs::datacount () const
{
  return static_cast<uint8_t> ((mAbstractcsReg & DATACOUNT_MASK)
                               >> DATACOUNT_OFFSET);
}

/// \brief Output operator for the Dmi::Abstractcs class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Abstractcs> &p)
{
  std::ostringstream oss;

  if (p->mPrettyPrint)
    oss << "[ progbufsize = " << static_cast<uint16_t> (p->progbufsize ())
        << ", busy = " << (p->busy () ? "true" : "false")
        << ", cmderr = " << static_cast<uint16_t> (p->cmderr ()) << " ("
        << (p->cmderr () == 0
                ? "none"
                : p->cmderr () == 1
                      ? "busy"
                      : p->cmderr () == 2
                            ? "not supported"
                            : p->cmderr () == 3
                                  ? "exception"
                                  : p->cmderr () == 4
                                        ? "halt/resume"
                                        : p->cmderr () == 5
                                              ? "bus"
                                              : p->cmderr () == 7 ? "other"
                                                                  : "INVALID")
        << ")"
        << ", datacount = 0x" << static_cast<uint16_t> (p->datacount ())
        << " ]";
  else
    oss << hex << setw (8) << setfill ('0') << p->mAbstractcsReg;

  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Command class                                                      *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Command class.
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Command::Command (unique_ptr<IDtm> &dtm_)
    : mPrettyPrint (false), mDtm (dtm_), mCommandReg (Dmi::Command::RESET_VALUE)
{
}

/// \brief Set the \c command register to its reset value.
void
Dmi::Command::reset ()
{
  mCommandReg = RESET_VALUE;
}

/// \brief Write the value of the \c command register.
///
/// The register is refreshed via the DTM, and we save the value read back.
void
Dmi::Command::write ()
{
  mCommandReg = mDtm->dmiWrite (DMI_ADDR, mCommandReg);
}

/// \brief Control whether to pretty print the \c command register.
///
/// @param[in] flag  If \c true, subsequent stream output will generate a list
///                  of field values, if false a simple hex value.
void
Dmi::Command::prettyPrint (const bool flag)
{
  mPrettyPrint = flag;
}

/// \brief Set the \c cmdtype bits of \c command
///
/// \param[in] cmdtypeVal  The value of \c cmdType to set.
void
Dmi::Command::cmdtype (const uint8_t cmdtypeVal)
{
  mCommandReg &= ~CMDTYPE_MASK;
  mCommandReg
      |= (static_cast<uint32_t> (cmdtypeVal) << CMDTYPE_OFFSET) & CMDTYPE_MASK;
}

/// \brief Set the \c control bits of \c command
///
/// \param[in] controlVal  The value of \c control to set.
void
Dmi::Command::control (const uint32_t controlVal)
{
  if (controlVal >= (1 << CONTROL_SIZE))
    cerr << "Warning: requested value of control, " << controlVal
         << ", exceeds the maximum permitted value: higher bits ignored."
         << endl;

  mCommandReg &= ~CONTROL_MASK;
  mCommandReg
      |= (static_cast<uint32_t> (controlVal) << CONTROL_OFFSET) & CONTROL_MASK;
}

/// \brief Output operator for the Dmi::Command class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Command> &p)
{
  std::ostringstream oss;

  if (p->mPrettyPrint)
    oss << "[ cmdtype = "
        << ((p->mCommandReg & p->CMDTYPE_MASK) >> p->CMDTYPE_OFFSET)
        << ", control = 0x" << hex << setw (6) << setfill ('0')
        << ((p->mCommandReg & p->CONTROL_MASK) >> p->CONTROL_OFFSET) << " ]";
  else
    oss << hex << setw (8) << setfill ('0') << p->mCommandReg;

  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Abstractauto class                                                 *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Abstractauto class.
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Abstractauto::Abstractauto (unique_ptr<IDtm> &dtm_)
    : mPrettyPrint (false), mDtm (dtm_),
      mAbstractautoReg (Dmi::Abstractauto::RESET_VALUE)
{
}

/// \brief Read the value of the \c abstractauto register.
///
/// The register is refreshed via the DTM.
void
Dmi::Abstractauto::read ()
{
  mAbstractautoReg = mDtm->dmiRead (DMI_ADDR);
}

/// \brief Set the \c abstractauto register to its reset value.
void
Dmi::Abstractauto::reset ()
{
  mAbstractautoReg = RESET_VALUE;
}

/// \brief Write the value of the \c abstractauto register.
///
/// The register is refreshed via the DTM, and we save the value read back.
void
Dmi::Abstractauto::write ()
{
  mAbstractautoReg = mDtm->dmiWrite (DMI_ADDR, mAbstractautoReg);
}

/// \brief Control whether to pretty print the \c abstractauto register.
///
/// @param[in] flag  If \c true, subsequent stream output will generate a list
///                  of field values, if false a simple hex value.
void
Dmi::Abstractauto::prettyPrint (const bool flag)
{
  mPrettyPrint = flag;
}

/// \brief Get the \c autoexecprogbuf bits of \c abstractauto.
///
/// \return  The value of the \c autoexecprogbuf bits of \c abstractauto
uint16_t
Dmi::Abstractauto::autoexecprogbuf () const
{
  return static_cast<uint16_t> ((mAbstractautoReg & AUTOEXECPROGBUF_MASK)
                                << AUTOEXECPROGBUF_OFFSET);
}

/// \brief Set the \c autoexecprogbuf bits of \c abstractauto.
///
/// \param[in] autoexecprogbufVal  The value of the \c autoexecprogbuf bits to
///                                set in \c abstractauto.
void
Dmi::Abstractauto::autoexecprogbuf (const uint16_t autoexecprogbufVal)
{
  mAbstractautoReg &= ~AUTOEXECPROGBUF_MASK;
  mAbstractautoReg
      |= (static_cast<uint32_t> (autoexecprogbufVal) << AUTOEXECPROGBUF_OFFSET)
         & AUTOEXECPROGBUF_MASK;
}

/// \brief Get the \c autoexecdata bits of \c abstractauto.
///
/// \return  The value of the \c autoexecdata bits of \c abstractauto
uint16_t
Dmi::Abstractauto::autoexecdata () const
{
  return static_cast<uint16_t> ((mAbstractautoReg & AUTOEXECDATA_MASK)
                                << AUTOEXECDATA_OFFSET);
}

/// \brief Set the \c autoexecdata bits of \c abstractauto.
///
/// \param[in] autoexecdataVal  The value of the \c autoexecdata bits to set in
///                        \c abstractauto.
void
Dmi::Abstractauto::autoexecdata (const uint16_t autoexecdataVal)
{
  if (autoexecdataVal >= (1 << AUTOEXECDATA_SIZE))
    cerr << "Warning: requested value of autoexecdata, " << autoexecdataVal
         << ", exceeds the maximum permitted value: higher bits ignored."
         << endl;

  mAbstractautoReg &= ~AUTOEXECDATA_MASK;
  mAbstractautoReg
      |= (static_cast<uint32_t> (autoexecdataVal) << AUTOEXECDATA_OFFSET)
         & AUTOEXECDATA_MASK;
}

/// \brief Output operator for the Dmi::Abstractauto class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Abstractauto> &p)
{
  std::ostringstream oss;

  if (p->mPrettyPrint)
    oss << "[ autoexecprogbuf = 0x" << hex << setw (4) << setfill ('0')
        << p->autoexecprogbuf () << ", autoexecdata = 0x" << setw (3)
        << p->autoexecdata () << " ]";
  else
    oss << hex << setw (8) << setfill ('0') << p->mAbstractautoReg;

  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Confstrptr class                                                   *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Confstrptr class
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Confstrptr::Confstrptr (unique_ptr<IDtm> &dtm_) : mDtm (dtm_)
{
  for (size_t i = 0; i < NUM_REGS; i++)
    mConfstrptrReg[i] = 0x0;
}

/// \brief Read the value of the specified \c confstrptr register.
///
/// The register is refreshed via the DTM.
///
/// \param[in] n  Index of the \c confstrptr register to read.
void
Dmi::Confstrptr::read (const size_t n)
{
  if (n < NUM_REGS)
    mConfstrptrReg[n] = mDtm->dmiRead (DMI_ADDR[n]);
  else
    cerr << "Warning: reading confstrptr[" << n << "] invalid: ignored."
         << endl;
}

/// \brief Must define as well as declare our private constexpr before using.
constexpr uint64_t Dmi::Confstrptr::DMI_ADDR[Dmi::Confstrptr::NUM_REGS];

/// Get the value the specified \c confstrptr register.
///
/// \param[in] n  Index of the \c confstrptr register to get.
/// \return  The value in the specified \c confstrptr register.
uint32_t
Dmi::Confstrptr::confstrptr (const size_t n) const
{
  if (n < NUM_REGS)
    return mConfstrptrReg[n];
  else
    {
      cerr << "Warning: getting confstrptr[" << n << "] invalid: zero returned."
           << endl;
      return 0;
    }
}

/// \brief Output operator for the Dmi::Confstrptr class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Confstrptr> &p)
{
  std::ostringstream oss;
  oss << "[" << hex << setw (8) << setfill ('0');

  for (size_t i = 0; i < p->NUM_REGS; i++)
    {
      oss << p->mConfstrptrReg[i];
      if (i != (p->NUM_REGS - 1))
        oss << ", ";
    }

  oss << "]";
  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Nextdm class                                                       *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Nextdm class.
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Nextdm::Nextdm (unique_ptr<IDtm> &dtm_)
    : mDtm (dtm_), mNextdmReg (Dmi::Nextdm::RESET_VALUE)
{
}

/// \brief Read the value of the \c nextdm register.
///
/// The register is refreshed via the DTM.
void
Dmi::Nextdm::read ()
{
  mNextdmReg = mDtm->dmiRead (DMI_ADDR);
}

/// \brief Get the value of \c nextdm
///
/// \return The value of \c nextdm
uint32_t
Dmi::Nextdm::nextdm () const
{
  return mNextdmReg;
}

/// \brief Output operator for the Dmi::Nextdm class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Nextdm> &p)
{
  std::ostringstream oss;
  oss << hex << setw (8) << setfill ('0') << p->mNextdmReg;
  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Progbuf class                                                      *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Progbuf class
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Progbuf::Progbuf (unique_ptr<IDtm> &dtm_) : mDtm (dtm_)
{
  for (size_t i = 0; i < NUM_REGS; i++)
    mProgbufReg[i] = 0x0;
}

/// \brief Read the value of the specified abstract \c progbuf register.
///
/// The register is refreshed via the DTM.
///
/// \param[in] n  Index of the \c progbuf register to read.
void
Dmi::Progbuf::read (const size_t n)
{
  if (n < NUM_REGS)
    mProgbufReg[n] = mDtm->dmiRead (DMI_ADDR[n]);
  else
    cerr << "Warning: reading progbuf[" << n << "] invalid: ignored." << endl;
}

/// \brief Set the specified abstract \c progbuf register to its reset value.
void
Dmi::Progbuf::reset (const size_t n)
{
  if (n < NUM_REGS)
    mProgbufReg[n] = RESET_VALUE;
  else
    cerr << "Warning: reseting progbuf[" << n << "] invalid: ignored." << endl;
}

/// \brief Write the value of the specified abstract \c progbuf register.
///
/// The register is refreshed via the DTM, and we save the value read back.
void
Dmi::Progbuf::write (const size_t n)
{
  if (n < NUM_REGS)
    mProgbufReg[n] = mDtm->dmiWrite (DMI_ADDR[n], mProgbufReg[n]);
  else
    cerr << "Warning: writing progbuf[" << n << "] invalid: ignored." << endl;
}

/// \brief Must define as well as declare our private constexpr before using.
constexpr uint64_t Dmi::Progbuf::DMI_ADDR[Dmi::Progbuf::NUM_REGS];

/// Get the value the specified \c progbuf register.
///
/// \param[in] n  Index of the \c progbuf register to get.
/// \return  The value in the specified \c progbuf register.
uint32_t
Dmi::Progbuf::progbuf (const size_t n) const
{
  if (n < NUM_REGS)
    return mProgbufReg[n];
  else
    {
      cerr << "Warning: getting progbuf[" << n << "] invalid: zero returned."
           << endl;
      return 0;
    }
}

/// Set the value the specified \c progbuf register.
///
/// \param[in] n         Index of the \c progbuf register to get.
/// \param[in] progbufValy  The value to be set in the specified \c progbuf
/// register.
void
Dmi::Progbuf::progbuf (const size_t n, const uint32_t progbufVal)
{
  if (n < NUM_REGS)
    mProgbufReg[n] = progbufVal;
  else
    cerr << "Warning: setting progbuf[" << n << "] invalid: ignored." << endl;
}

/// \brief Output operator for the Dmi::Progbuf class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Progbuf> &p)
{
  std::ostringstream oss;
  oss << "[" << hex << setw (8) << setfill ('0');

  for (size_t i = 0; i < p->NUM_REGS; i++)
    {
      oss << p->mProgbufReg[i];
      if (i != (p->NUM_REGS - 1))
        oss << ", ";
    }

  oss << "]";
  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Authdata class                                                     *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Authdata class.
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Authdata::Authdata (unique_ptr<IDtm> &dtm_)
    : mDtm (dtm_), mAuthdataReg (Dmi::Authdata::RESET_VALUE)
{
}

/// \brief Read the value of the \c authdata register.
///
/// The register is refreshed via the DTM.
void
Dmi::Authdata::read ()
{
  cerr << "Warning: authentication not supported while reading authdata"
       << endl;
  mAuthdataReg = mDtm->dmiRead (DMI_ADDR);
}

/// \brief Set the \c authdata register to its reset value.
void
Dmi::Authdata::reset ()
{
  cerr << "Warning: authentication not supported while reseting authdata"
       << endl;
  mAuthdataReg = RESET_VALUE;
}

/// \brief Write the value of the \c authdata register.
///
/// The register is refreshed via the DTM, and we save the value read back.
void
Dmi::Authdata::write ()
{
  cerr << "Warning: authentication not supported while writing authdata"
       << endl;
  mAuthdataReg = mDtm->dmiWrite (DMI_ADDR, mAuthdataReg);
}

/// \brief Get the value of \c authdata
///
/// \return The value of \c authdata
uint32_t
Dmi::Authdata::authdata () const
{
  cerr << "Warning: authentication not supported while getting authdata"
       << endl;
  return mAuthdataReg;
}

/// \brief Set the value of \c authdata
///
/// \param[in] authdataVal  The value of \c authdata to set.
void
Dmi::Authdata::authdata (const uint32_t authdataVal __attribute__ ((unused)))
{
  cerr << "Warning: authentication not supported while setting authdata: "
       << "value ignored" << endl;
}

/// \brief Output operator for the Dmi::Authdata class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Authdata> &p)
{
  std::ostringstream oss;
  oss << hex << setw (8) << setfill ('0') << p->mAuthdataReg;
  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Sbaddress class                                                    *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Sbaddress class
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Sbaddress::Sbaddress (unique_ptr<IDtm> &dtm_) : mDtm (dtm_)
{
  for (size_t i = 0; i < NUM_REGS; i++)
    mSbaddressReg[i] = 0x0;
}

/// \brief Read the value of the specified abstract \c sbaddress register.
///
/// The register is refreshed via the DTM.
///
/// \param[in] n  Index of the \c sbaddress register to read.
void
Dmi::Sbaddress::read (const size_t n)
{
  if (n < NUM_REGS)
    mSbaddressReg[n] = mDtm->dmiRead (DMI_ADDR[n]);
  else
    cerr << "Warning: reading sbaddress[" << n << "] invalid: ignored." << endl;
}

/// \brief Set the specified abstract \c sbaddress register to its reset value.
void
Dmi::Sbaddress::reset (const size_t n)
{
  if (n < NUM_REGS)
    mSbaddressReg[n] = RESET_VALUE;
  else
    cerr << "Warning: reseting sbaddress[" << n << "] invalid: ignored."
         << endl;
}

/// \brief Write the value of the specified abstract \c sbaddress register.
///
/// The register is refreshed via the DTM, and we save the value read back.
void
Dmi::Sbaddress::write (const size_t n)
{
  if (n < NUM_REGS)
    mSbaddressReg[n] = mDtm->dmiWrite (DMI_ADDR[n], mSbaddressReg[n]);
  else
    cerr << "Warning: writing sbaddress[" << n << "] invalid: ignored." << endl;
}

/// \brief Must define as well as declare our private constexpr before using.
constexpr uint64_t Dmi::Sbaddress::DMI_ADDR[Dmi::Sbaddress::NUM_REGS];

/// Get the value the specified \c sbaddress register.
///
/// \param[in] n  Index of the \c sbaddress register to get.
/// \return  The value in the specified \c sbaddress register.
uint32_t
Dmi::Sbaddress::sbaddress (const size_t n) const
{
  if (n < NUM_REGS)
    return mSbaddressReg[n];
  else
    {
      cerr << "Warning: getting sbaddress[" << n << "] invalid: zero returned."
           << endl;
      return 0;
    }
}

/// Set the value the specified \c sbaddress register.
///
/// \param[in] n         Index of the \c sbaddress register to get.
/// \param[in] sbaddressValy  The value to be set in the specified \c sbaddress
/// register.
void
Dmi::Sbaddress::sbaddress (const size_t n, const uint32_t sbaddressVal)
{
  if (n < NUM_REGS)
    mSbaddressReg[n] = sbaddressVal;
  else
    cerr << "Warning: setting sbaddress[" << n << "] invalid: ignored." << endl;
}

/// \brief Output operator for the Dmi::Sbaddress class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Sbaddress> &p)
{
  std::ostringstream oss;
  oss << "[" << hex << setw (8) << setfill ('0');

  for (size_t i = 0; i < p->NUM_REGS; i++)
    {
      oss << p->mSbaddressReg[i];
      if (i != (p->NUM_REGS - 1))
        oss << ", ";
    }

  oss << "]";
  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Sbcs class                                                         *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Sbcs class.
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Sbcs::Sbcs (unique_ptr<IDtm> &dtm_)
    : mPrettyPrint (false), mDtm (dtm_), mSbcsReg (Dmi::Sbcs::RESET_VALUE)
{
}

/// \brief Read the value of the \c sbcs register.
///
/// The register is refreshed via the DTM.
void
Dmi::Sbcs::read ()
{
  mSbcsReg = mDtm->dmiRead (DMI_ADDR);
}

/// \brief Set the \c sbcs register to its reset value.
void
Dmi::Sbcs::reset ()
{
  mSbcsReg = RESET_VALUE;
}

/// \brief Write the value of the \c sbcs register.
///
/// The register is refreshed via the DTM, and we save the value read back.
void
Dmi::Sbcs::write ()
{
  mSbcsReg = mDtm->dmiWrite (DMI_ADDR, mSbcsReg);
}

/// \brief Control whether to pretty print the \c sbcs register.
///
/// @param[in] flag  If \c true, subsequent stream output will generate a list
///                  of field values, if false a simple hex value.
void
Dmi::Sbcs::prettyPrint (const bool flag)
{
  mPrettyPrint = flag;
}

/// \brief Get the \c sbversion bits in \c sbcs.
///
/// \return  The value in the \c sbversion bits of \c sbcs.
uint8_t
Dmi::Sbcs::sbversion () const
{
  return static_cast<uint8_t> ((mSbcsReg | SBVERSION_MASK) >> SBVERSION_OFFSET);
}

/// \brief Get the \c sbbusyerror bit in \c sbcs.
///
/// \return  \c true if the \c sbbusyerror bit of \c sbcs is set, \c false
///          otherwise.
bool
Dmi::Sbcs::sbbusyerror () const
{
  return (mSbcsReg & SBBUSYERROR_MASK) != 0;
}

/// \brief Clear the \c sbbusyerror bit in \c sbcs.
///
/// Writing 1 to this bit clears it.
void
Dmi::Sbcs::sbbusyerrorClear ()
{
  mSbcsReg |= SBBUSYERROR_MASK;
}

/// \brief Get the \c sbbusy bit in \c sbcs.
///
/// \return  \c true if the \c sbbusy bit of \c sbcs is set, \c false
///          otherwise.
bool
Dmi::Sbcs::sbbusy () const
{
  return (mSbcsReg & SBBUSY_MASK) != 0;
}

/// \brief Get the \c sbreadonaddr bit in \c sbcs.
///
/// \return  \c true if the \c sbreadonaddr bit of \c sbcs is set, \c false
///          otherwise.
bool
Dmi::Sbcs::sbreadonaddr () const
{
  return (mSbcsReg & SBREADONADDR_MASK) != 0;
}

/// \brief Set the \c sbreadonaddr bit in \c sbcs.
///
/// \param[in] flag  If \c true sets \c sbreadonaddr bit in \c sbcs, otherwise
///                  clears it.
void
Dmi::Sbcs::sbreadonaddr (const bool flag)
{
  if (flag)
    mSbcsReg |= SBREADONADDR_MASK;
  else
    mSbcsReg &= ~SBREADONADDR_MASK;
}

/// \brief Get the \c sbaccess bits of \c sbcs.
///
/// \return  The value of the \c sbaccess bits of \c sbcs.
uint8_t
Dmi::Sbcs::sbaccess () const
{
  return static_cast<uint8_t> ((mSbcsReg | SBACCESS_MASK) >> SBACCESS_OFFSET);
}

/// \brief Set the \c sbaccess bits in \c sbcs.
///
/// \param[in] val  Value to set for the \c sb bits in \c sbcs.
void
Dmi::Sbcs::sbaccess (const uint8_t val)
{
  if (val >= (1 << SBACCESS_SIZE))
    cerr << "Warning: " << val << " too large for sbaccess field of sbcs: "
         << "truncated" << endl;
  mSbcsReg &= ~SBACCESS_MASK;
  mSbcsReg |= (static_cast<uint32_t> (val) << SBACCESS_OFFSET) & SBACCESS_MASK;
}

/// \brief Get the \c sbautoincrement bit in \c sbcs.
///
/// \return  \c true if the \c sbautoincrement bit of \c sbcs is set, \c false
///          otherwise.
bool
Dmi::Sbcs::sbautoincrement () const
{
  return (mSbcsReg & SBAUTOINCREMENT_MASK) != 0;
}

/// \brief Set the \c sbautoincrement bit in \c sbcs.
///
/// \param[in] flag  If \c true sets \c sbautoincrement bit in \c sbcs,
///                  otherwise clears it.
void
Dmi::Sbcs::sbautoincrement (const bool flag)
{
  if (flag)
    mSbcsReg |= SBAUTOINCREMENT_MASK;
  else
    mSbcsReg &= ~SBAUTOINCREMENT_MASK;
}

/// \brief Get the \c sbreadondata bit in \c sbcs.
///
/// \return  \c true if the \c sbreadondata bit of \c sbcs is set, \c false
///          otherwise.
bool
Dmi::Sbcs::sbreadondata () const
{
  return (mSbcsReg & SBREADONDATA_MASK) != 0;
}

/// \brief Set the \c sbreadondata bit in \c sbcs.
///
/// \param[in] flag  If \c true sets \c sbreadondata bit in \c sbcs, otherwise
///                  clears it.
void
Dmi::Sbcs::sbreadondata (const bool flag)
{
  if (flag)
    mSbcsReg |= SBREADONDATA_MASK;
  else
    mSbcsReg &= ~SBREADONDATA_MASK;
}

/// \brief Get the \c sberror bits in \c sbcs.
///
/// \return  The value of the \c sberror bits in \c sbcs.
uint8_t
Dmi::Sbcs::sberror () const
{
  return static_cast<uint8_t> ((mSbcsReg | SBERROR_MASK) >> SBERROR_OFFSET);
}

/// \brief Clear the \c sberror bits in \c sbcs.
///
/// Writing 1 to these bits clears the error.
void
Dmi::Sbcs::sberrorClear ()
{
  mSbcsReg |= SBERROR_MASK;
}

/// \brief Get the \c sbasize bits in \c sbcs.
///
/// \return  The value of the \c sbasize bits in \c sbcs.
uint8_t
Dmi::Sbcs::sbasize () const
{
  return static_cast<uint8_t> ((mSbcsReg | SBASIZE_MASK) >> SBASIZE_OFFSET);
}

/// \brief Get the \c sbaccess128 bit in \c sbcs.
///
/// \return  \c true if the \c sbaccess128 bit of \c sbcs is set, \c false
///          otherwise.
bool
Dmi::Sbcs::sbaccess128 () const
{
  return (mSbcsReg & SBACCESS128_MASK) != 0;
}

/// \brief Get the \c sbaccess64 bit in \c sbcs.
///
/// \return  \c true if the \c sbaccess64 bit of \c sbcs is set, \c false
///          otherwise.
bool
Dmi::Sbcs::sbaccess64 () const
{
  return (mSbcsReg & SBACCESS64_MASK) != 0;
}

/// \brief Get the \c sbaccess32 bit in \c sbcs.
///
/// \return  \c true if the \c sbaccess32 bit of \c sbcs is set, \c false
///          otherwise.
bool
Dmi::Sbcs::sbaccess32 () const
{
  return (mSbcsReg & SBACCESS32_MASK) != 0;
}

/// \brief Get the \c sbaccess16 bit in \c sbcs.
///
/// \return  \c true if the \c sbaccess16 bit of \c sbcs is set, \c false
///          otherwise.
bool
Dmi::Sbcs::sbaccess16 () const
{
  return (mSbcsReg & SBACCESS16_MASK) != 0;
}

/// \brief Get the \c sbaccess8 bit in \c sbcs.
///
/// \return  \c true if the \c sbaccess8 bit of \c sbcs is set, \c false
///          otherwise.
bool
Dmi::Sbcs::sbaccess8 () const
{
  return (mSbcsReg & SBACCESS8_MASK) != 0;
}

/// \brief Output operator for the Dmi::Sbcs class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Sbcs> &p)
{
  std::ostringstream oss;

  if (p->mPrettyPrint)
    oss << "[ sbversion = " << static_cast<uint16_t> (p->sbversion ()) << " ("
        << (p->sbversion () == 0
                ? "pre 1 Jan 2019"
                : p->sbversion () == 1 ? "debug spec 0.13.2" : "RESERVED")
        << ")"
        << ", sbbusyerror = " << (p->sbbusyerror () ? "true" : "false")
        << ", sbbusy = " << (p->sbbusy () ? "true" : "false")
        << ", sbreadonaddr = " << (p->sbreadonaddr () ? "true" : "false")
        << ", sbaccess = " << static_cast<uint16_t> (p->sbaccess ()) << " ("
        << (p->sbaccess () == 0
                ? "8-bit"
                : p->sbaccess () == 1
                      ? "16-bit"
                      : p->sbaccess () == 2
                            ? "32-bit"
                            : p->sbaccess () == 3
                                  ? "64-bit"
                                  : p->sbaccess () == 4 ? "128-bit" : "INVALID")
        << ")"
        << ", sbautoincrement = " << (p->sbautoincrement () ? "true" : "false")
        << ", sbreadondata = " << (p->sbreadondata () ? "true" : "false")
        << ", sberror = " << static_cast<uint16_t> (p->sberror ()) << " ("
        << (p->sberror () == 0
                ? "none"
                : p->sberror () == 1
                      ? "timeout"
                      : p->sberror () == 2
                            ? "bad address"
                            : p->sberror () == 3
                                  ? "alignment"
                                  : p->sberror () == 4
                                        ? "bad size"
                                        : p->sberror () == 7 ? "other"
                                                             : "INVALID")
        << ")"
        << ", sbasize = " << static_cast<uint16_t> (p->sbasize ())
        << ", sbaccess128 = " << (p->sbaccess128 () ? "true" : "false")
        << ", sbaccess64 = " << (p->sbaccess64 () ? "true" : "false")
        << ", sbaccess32 = " << (p->sbaccess32 () ? "true" : "false")
        << ", sbaccess16 = " << (p->sbaccess16 () ? "true" : "false")
        << ", sbaccess8 = " << (p->sbaccess8 () ? "true" : "false") << " ]";
  else
    oss << hex << setw (8) << setfill ('0') << p->mSbcsReg;

  return s << oss.str ();
}

/*******************************************************************************
 *                                                                             *
 * The Dmi::Sbdata class                                                       *
 *                                                                             *
 ******************************************************************************/

/// \brief constructor for the Dmi::Sbdata class
///
/// \param[in] dtm_  The DTM we shall use.  This is a unique_ptr owned by the
///                  Dmi, so passed and stored by reference.
Dmi::Sbdata::Sbdata (unique_ptr<IDtm> &dtm_) : mDtm (dtm_)
{
  for (size_t i = 0; i < NUM_REGS; i++)
    mSbdataReg[i] = 0x0;
}

/// \brief Read the value of the specified abstract \c sbdata register.
///
/// The register is refreshed via the DTM.
///
/// \param[in] n  Index of the \c sbdata register to read.
void
Dmi::Sbdata::read (const size_t n)
{
  if (n < NUM_REGS)
    mSbdataReg[n] = mDtm->dmiRead (DMI_ADDR[n]);
  else
    cerr << "Warning: reading sbdata[" << n << "] invalid: ignored." << endl;
}

/// \brief Set the specified abstract \c sbdata register to its reset value.
void
Dmi::Sbdata::reset (const size_t n)
{
  if (n < NUM_REGS)
    mSbdataReg[n] = RESET_VALUE;
  else
    cerr << "Warning: reseting sbdata[" << n << "] invalid: ignored." << endl;
}

/// \brief Write the value of the specified abstract \c sbdata register.
///
/// The register is refreshed via the DTM, and we save the value read back.
void
Dmi::Sbdata::write (const size_t n)
{
  if (n < NUM_REGS)
    mSbdataReg[n] = mDtm->dmiWrite (DMI_ADDR[n], mSbdataReg[n]);
  else
    cerr << "Warning: writing sbdata[" << n << "] invalid: ignored." << endl;
}

/// \brief Must define as well as declare our private constexpr before using.
constexpr uint64_t Dmi::Sbdata::DMI_ADDR[Dmi::Sbdata::NUM_REGS];

/// Get the value the specified \c sbdata register.
///
/// \param[in] n  Index of the \c sbdata register to get.
/// \return  The value in the specified \c sbdata register.
uint32_t
Dmi::Sbdata::sbdata (const size_t n) const
{
  if (n < NUM_REGS)
    return mSbdataReg[n];
  else
    {
      cerr << "Warning: getting sbdata[" << n << "] invalid: zero returned."
           << endl;
      return 0;
    }
}

/// Set the value the specified \c sbdata register.
///
/// \param[in] n         Index of the \c sbdata register to get.
/// \param[in] sbdataValy  The value to be set in the specified \c sbdata
/// register.
void
Dmi::Sbdata::sbdata (const size_t n, const uint32_t sbdataVal)
{
  if (n < NUM_REGS)
    mSbdataReg[n] = sbdataVal;
  else
    cerr << "Warning: setting sbdata[" << n << "] invalid: ignored." << endl;
}

/// \brief Output operator for the Dmi::Sbdata class
///
/// \param[in] s  The stream to which output is written
/// \param[in] p  A unique_ptr to the instance to output
/// \return  The stream with the instance appended
std::ostream &
operator<< (ostream &s, const unique_ptr<Dmi::Sbdata> &p)
{
  std::ostringstream oss;
  oss << "[" << hex << setw (8) << setfill ('0');

  for (size_t i = 0; i < p->NUM_REGS; i++)
    {
      oss << p->mSbdataReg[i];
      if (i != (p->NUM_REGS - 1))
        oss << ", ";
    }

  oss << "]";
  return s << oss.str ();
}

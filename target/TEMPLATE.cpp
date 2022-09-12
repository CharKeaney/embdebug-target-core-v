// TEMPLATE ITarget interface implementation
//
// This file is part of the Embecosm Debug Server targets.
//
// Copyright (C) 2022 Embecosm Limited
// SPDX-License-Identifier: MIT
// ----------------------------------------------------------------------------

#include "TEMPLATE.h"
#include "embdebug/Compat.h"
#include "embdebug/ITarget.h"
#include "Args.h"
#include "DtmJtag.h"

#include <iostream>
using namespace EmbDebug;

// Instantiate the model. TODO the argument will change to pass in the
// residual argv.
TEMPLATE::TEMPLATE(const TraceFlags *traceFlags EMBDEBUG_ATTR_UNUSED)
  : ITarget(traceFlags) { 
  mDtm = std::make_unique<DtmJtag>(100, 0, "a");
  mDmi = std::make_unique<Dmi>(std::move (mDtm));
  return; 
}

// Clean up the model
TEMPLATE::~TEMPLATE() { return; }

// Force termination of the model execution
ITarget::ResumeRes TEMPLATE::terminate() { return ITarget::ResumeRes::NONE; }

// Reset the model state
ITarget::ResumeRes TEMPLATE::reset(ITarget::ResetType) {
  return ITarget::ResumeRes::SUCCESS;
}

// Count cycles
uint64_t TEMPLATE::getCycleCount() const { return mDmi->simTimeCountNs(); }

// Count instructions
uint64_t TEMPLATE::getInstrCount() const { return 0; }

// How many registers do we have?
int TEMPLATE::getRegisterCount() const { return 32; }

// How big is each register in bytes
int TEMPLATE::getRegisterSize() const { return 4; }

// Read a register into the VALUE reference argument, returning the number of
// bytes read
std::size_t TEMPLATE::readRegister(const int reg EMBDEBUG_ATTR_UNUSED,
				   uint_reg_t &value) {
  uint32_t reg_result;
  mDmi->readGpr(reg, reg_result);
  value = reg_result;
  return getRegisterSize();
}

// This is the mechanism for handling host file I/O. It provides a mechanism
// for supplying arguments and returning results. In this template we report
// that the syscall number is in register 1, the arguments in registers 2-4
// and the result in register 5. Consult your ABI to determine the correct
// values for a real architecture. Return if this was successful or not.
bool TEMPLATE::getSyscallArgLocs(SyscallArgLoc &syscallIDLoc,
				 std::vector<SyscallArgLoc> &syscallArgLocs,
				 SyscallArgLoc &syscallReturnLoc) const {
  syscallIDLoc = SyscallArgLoc::RegisterLoc({SyscallArgLocType::REGISTER, 1});

  syscallArgLocs.clear();
  syscallArgLocs.push_back(SyscallArgLoc::RegisterLoc({SyscallArgLocType::REGISTER, 2}));
  syscallArgLocs.push_back(SyscallArgLoc::RegisterLoc({SyscallArgLocType::REGISTER, 3}));
  syscallArgLocs.push_back(SyscallArgLoc::RegisterLoc({SyscallArgLocType::REGISTER, 4}));

  // Return value in a0
  syscallReturnLoc = SyscallArgLoc::RegisterLoc({SyscallArgLocType::REGISTER, 5});
  return true;
}

// Write a register supplied in the VALUE argument and returning the number of
// bytes written
std::size_t TEMPLATE::writeRegister(const int reg EMBDEBUG_ATTR_UNUSED,
				    const uint_reg_t value EMBDEBUG_ATTR_UNUSED) {
  mDmi->writeGpr(reg, value);
  return getRegisterSize();
}

// Read a block of memory into the supplied buffer, returning the number of
// bytes read
std::size_t TEMPLATE::read(const uint_addr_t addr EMBDEBUG_ATTR_UNUSED,
			   uint8_t *buffer EMBDEBUG_ATTR_UNUSED,
			   const std::size_t size) {
  std::unique_ptr<std::uint8_t[]> buf (new std::uint8_t[size]);
  mDmi->readMem(addr, size, buf);
  std::memcpy(buffer, buf.get(), size);
  return size;
}

// Write a block of memory from the supplied buffer, returning the number of
// bytes read
std::size_t TEMPLATE::write(const uint_addr_t addr EMBDEBUG_ATTR_UNUSED,
			    const uint8_t *buffer EMBDEBUG_ATTR_UNUSED,
			    const std::size_t size) {
  std::unique_ptr<std::uint8_t[]> buf (new std::uint8_t[size]);
  std::memcpy(buf.get(), buffer, size);
  mDmi->writeMem(addr, size, buf);
  return size;
}

// Insert a matchpoint (breakpoint or watchpoint), returning whether or not
// this succeeded
bool TEMPLATE::insertMatchpoint(const uint_addr_t EMBDEBUG_ATTR_UNUSED addr,
				const MatchType EMBDEBUG_ATTR_UNUSED matchType) {
  return false;
}

// Delete a matchpoint (breakpoint or watchpoint), returning whether or not
// this succeeded
bool TEMPLATE::removeMatchpoint(const uint_addr_t EMBDEBUG_ATTR_UNUSED addr,
                             const MatchType EMBDEBUG_ATTR_UNUSED matchType) {
  return false;
}

// Passthru' a command to the target, returning whether or not this succeeded.
bool TEMPLATE::command(const std::string EMBDEBUG_ATTR_UNUSED cmd,
		       std::ostream EMBDEBUG_ATTR_UNUSED &stream) {
  return false;
}

// Return the time taken by the CPU so far in seconds
double TEMPLATE::timeStamp() { return (double) mDmi->simTimeCountNs() / 1000000000; }

// Return the number of CPUs
unsigned int TEMPLATE::getCpuCount(void) { return 1; }

// Return the curent CPU (must be consistent with the number of CPUs, use -1
// to indicate an invalid response
unsigned int TEMPLATE::getCurrentCpu(void) { return 0; }

// Specify the current CPU
void TEMPLATE::setCurrentCpu(unsigned int num) {
  assert(num == 0);
  return;
}


// Prepare each core to be resumed. The supplied vector, ACTIONS, says what
// each core should do when resume is next called. Return whether this
// succeeded.
bool TEMPLATE::prepare(const std::vector<ITarget::ResumeType> &actions EMBDEBUG_ATTR_UNUSED) {
  return true;
}

// Resume each core, according to what was specified in the earlier call to
// prepare. Return whether this succeeded.
bool TEMPLATE::resume(void) {
  return true;
}

// Clock the model waiting for any core to stop
ITarget::WaitRes TEMPLATE::wait(std::vector<ResumeRes> &results EMBDEBUG_ATTR_UNUSED) {
  return ITarget::WaitRes::TIMEOUT;
}

// Force all cores to halt, returning whether this was successful
bool TEMPLATE::halt(void) { return true; }

// Entry point for the shared library
extern "C" {
// Create and return a new model
EMBDEBUG_VISIBLE_API ITarget *create_target(TraceFlags *traceFlags) {
  return new TEMPLATE(traceFlags);
}
// Used to ensure API compatibility
EMBDEBUG_VISIBLE_API uint64_t ITargetVersion() {
  return ITarget::CURRENT_API_VERSION;
}
}

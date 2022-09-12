// TEMPLATE ITarget interface implementation
//
// This file is part of the Embecosm Debug Server targets.
//
// Copyright (C) 2020 Embecosm Limited
// SPDX-License-Identifier: MIT
// ----------------------------------------------------------------------------

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "embdebug/ITarget.h"
#include "IDtm.h"
#include "Dmi.h"

using namespace EmbDebug;

class TEMPLATE : public ITarget {
public:
  TEMPLATE() = delete;
  TEMPLATE(const TEMPLATE &) = delete;

  explicit TEMPLATE(const TraceFlags *traceFlags);
  ~TEMPLATE();

  ITarget::ResumeRes terminate() override;
  ITarget::ResumeRes reset(ITarget::ResetType) override;

  uint64_t getCycleCount() const override;
  uint64_t getInstrCount() const override;

  int getRegisterCount() const override;
  int getRegisterSize() const override;
  bool getSyscallArgLocs(SyscallArgLoc &syscallIDLoc,
                         std::vector<SyscallArgLoc> &syscallArgLocs,
                         SyscallArgLoc &syscallReturnLoc) const override;

  std::size_t readRegister(const int reg, uint_reg_t &value) override;
  std::size_t writeRegister(const int reg, const uint_reg_t value) override;

  std::size_t read(const uint_addr_t addr, uint8_t *buffer,
                   const std::size_t size) override;

  std::size_t write(const uint_addr_t addr, const uint8_t *buffer,
                    const std::size_t size) override;

  bool insertMatchpoint(const uint_addr_t addr,
                        const MatchType matchType) override;
  bool removeMatchpoint(const uint_addr_t addr,
                        const MatchType matchType) override;

  bool command(const std::string cmd, std::ostream &stream) override;

  double timeStamp() override;

  unsigned int getCpuCount(void) override;
  unsigned int getCurrentCpu(void) override;
  void setCurrentCpu(unsigned int) override;
  bool prepare(const std::vector<ITarget::ResumeType> &) override;
  bool resume(void) override;
  ITarget::WaitRes wait(std::vector<ResumeRes> &) override;
  bool halt(void) override;

  bool supportsTargetXML(void) override { return false; }
  const char *getTargetXML(ByteView name EMBDEBUG_ATTR_UNUSED) override {
	  return nullptr;
  }

private:

  // No private members needed for the template target. This is where you
  // would place state and helper functions for a real target.
  std::unique_ptr<IDtm> mDtm;
  std::unique_ptr<Dmi> mDmi;
};

#endif

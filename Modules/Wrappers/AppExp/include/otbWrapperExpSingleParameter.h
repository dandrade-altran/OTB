/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbWrapperExpSingleParameter_h
#define otbWrapperExpSingleParameter_h

#include "otbWrapperExpTypes.h"
#include "otbWrapperExpParameter.h"
#include "OTBApplicationEngineExport.h"

namespace otb
{
namespace WrapperExp
{

class OTBApplicationEngine_EXPORT SingleParameter : 
public Parameter , private boost::noncopyable
{
public:
  typedef SingleParameter                      Self;
  typedef Parameter                            Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  itkTypeMacro( SingleParameter , Parameter );

  virtual const std::string & GetLitteralValue() const = 0;
  virtual float GetFloattingValue() const = 0;
  virtual int GetIntegerValue() const = 0;

protected:
  SingleParameter() = default ;
  ~SingleParameter() override = default ;

private:
};

} // end namespace WrapperExp
} // end namespace otb

#endif

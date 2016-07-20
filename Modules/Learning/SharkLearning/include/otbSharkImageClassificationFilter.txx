/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbSharkImageClassificationFilter_txx
#define otbSharkImageClassificationFilter_txx

#include "otbSharkImageClassificationFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TMaskImage>
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::SharkImageClassificationFilter()
{
  this->SetNumberOfIndexedInputs(2);
  this->SetNumberOfRequiredInputs(1);
  m_DefaultLabel = itk::NumericTraits<LabelType>::ZeroValue();

  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0,TOutputImage::New());
  this->SetNthOutput(1,ConfidenceImageType::New());
  m_UseConfidenceMap = false;
  m_Batch = false;
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::SetInputMask(const MaskImageType * mask)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<MaskImageType *>(mask));
}

template <class TInputImage, class TOutputImage, class TMaskImage>
const typename SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::MaskImageType *
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::GetInputMask()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return ITK_NULLPTR;
    }
  return static_cast<const MaskImageType *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TOutputImage, class TMaskImage>
typename SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::ConfidenceImageType *
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::GetOutputConfidence()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return ITK_NULLPTR;
    }
  return static_cast<ConfidenceImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::BeforeThreadedGenerateData()
{
  if (!m_Model)
    {
    itkGenericExceptionMacro(<< "No model for classification");
    }
  if(m_Batch)
    {
    // The calls to m_Model->SetInputListSample() and
    // m_Model->SetTargetListSample() make the model non thread safe.
    // OpenMP allows to speed things up (configure with
    // CMAKE_CXX_FLAGS:STRING=-fopenmp)
    this->SetNumberOfThreads(1);
    }
}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::ClassicThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Get the input pointers
  InputImageConstPointerType inputPtr     = this->GetInput();
  MaskImageConstPointerType  inputMaskPtr  = this->GetInputMask();
  OutputImagePointerType     outputPtr    = this->GetOutput();
  ConfidenceImagePointerType confidencePtr = this->GetOutputConfidence();

  // Progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Define iterators
  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionConstIterator<MaskImageType>  MaskIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;
  typedef itk::ImageRegionIterator<ConfidenceImageType> ConfidenceMapIteratorType;

  InputIteratorType inIt(inputPtr, outputRegionForThread);
  OutputIteratorType outIt(outputPtr, outputRegionForThread);

  // Eventually iterate on masks
  MaskIteratorType maskIt;
  if (inputMaskPtr)
    {
    maskIt = MaskIteratorType(inputMaskPtr, outputRegionForThread);
    maskIt.GoToBegin();
    }

  // setup iterator for confidence map
  bool computeConfidenceMap(m_UseConfidenceMap && m_Model->HasConfidenceIndex() && !m_Model->GetRegressionMode());
  ConfidenceMapIteratorType confidenceIt;
  if (computeConfidenceMap)
    {
    confidenceIt = ConfidenceMapIteratorType(confidencePtr,outputRegionForThread);
    confidenceIt.GoToBegin();
    }

  bool validPoint = true;
  double confidenceIndex = 0.0;

  // Walk the part of the image
  for (inIt.GoToBegin(), outIt.GoToBegin(); !inIt.IsAtEnd() && !outIt.IsAtEnd(); ++inIt, ++outIt)
    {
    // Check pixel validity
    if (inputMaskPtr)
      {
      validPoint = maskIt.Get() > 0;
      ++maskIt;
      }
    // If point is valid
    if (validPoint)
      {
      // Classifify
      if (computeConfidenceMap)
        {
        outIt.Set(m_Model->Predict(inIt.Get(),&confidenceIndex)[0]);
        }
      else
        {
        outIt.Set(m_Model->Predict(inIt.Get())[0]);
        }
      }
    else
      {
      // else, set default value
      outIt.Set(m_DefaultLabel);
      confidenceIndex = 0.0;
      }
    if (computeConfidenceMap)
      {
      confidenceIt.Set(confidenceIndex);
      ++confidenceIt;
      }
    progress.CompletedPixel();
    }

}

template <class TInputImage, class TOutputImage, class TMaskImage>
void
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::BatchThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  bool computeConfidenceMap(m_UseConfidenceMap && m_Model->HasConfidenceIndex() 
                            && !m_Model->GetRegressionMode());
  // Get the input pointers
  InputImageConstPointerType inputPtr     = this->GetInput();
  MaskImageConstPointerType  inputMaskPtr  = this->GetInputMask();
  OutputImagePointerType     outputPtr    = this->GetOutput();
  ConfidenceImagePointerType confidencePtr = this->GetOutputConfidence();
    
  // Progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Define iterators
  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionConstIterator<MaskImageType>  MaskIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;
  typedef itk::ImageRegionIterator<ConfidenceImageType> ConfidenceMapIteratorType;

  InputIteratorType inIt(inputPtr, outputRegionForThread);
  OutputIteratorType outIt(outputPtr, outputRegionForThread);

  MaskIteratorType maskIt;
  if (inputMaskPtr)
    {
    maskIt = MaskIteratorType(inputMaskPtr, outputRegionForThread);
    maskIt.GoToBegin();
    }

  typedef typename ModelType::InputValueType       InputValueType;
  typedef typename ModelType::InputSampleType      InputSampleType;
  typedef typename ModelType::InputListSampleType  InputListSampleType;
  typedef typename ModelType::TargetValueType      TargetValueType;
  typedef typename ModelType::TargetSampleType     TargetSampleType;
  typedef typename ModelType::TargetListSampleType TargetListSampleType;
  typedef typename ModelType::ConfidenceValueType      ConfidenceValueType;
  typedef typename ModelType::ConfidenceSampleType     ConfidenceSampleType;
  typedef typename ModelType::ConfidenceListSampleType ConfidenceListSampleType;

  typename InputListSampleType::Pointer samples = InputListSampleType::New();
  auto num_features = inputPtr->GetNumberOfComponentsPerPixel();
  samples->SetMeasurementVectorSize(num_features);
  InputSampleType sample(num_features);
  // Fill the samples
  bool validPoint = true;
  for (inIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt)
    {
    // Check pixel validity
    if (inputMaskPtr)
      {
      validPoint = maskIt.Get() > 0;
      ++maskIt;
      }
    if(validPoint)
      {
      auto pix = inIt.Get();
      for(size_t feat=0; feat<num_features; ++feat)
        {
        sample[feat]=pix[feat];
        }
      samples->PushBack(sample);
      }
    }
  //Make the batch prediction
  m_Model->SetInputListSample(samples);
  typename TargetListSampleType::Pointer labels = TargetListSampleType::New();
  typename ConfidenceListSampleType::Pointer confidences = ConfidenceListSampleType::New();
  if (computeConfidenceMap)
    {
    m_Model->SetConfidenceBatchMode(true);
    m_Model->SetConfidenceListSample(confidences);
    }
  m_Model->SetTargetListSample(labels);
  m_Model->PredictAll();
  // Set the output values
  ConfidenceMapIteratorType confidenceIt;
  if (computeConfidenceMap)
    {
    confidenceIt = ConfidenceMapIteratorType(confidencePtr,outputRegionForThread);
    confidenceIt.GoToBegin();
    }

  auto labIt = labels->Begin();
  auto confIt = confidences->Begin();
  maskIt.GoToBegin();
  for (outIt.GoToBegin(); labIt!=labels->End() && !outIt.IsAtEnd(); 
       ++outIt)
    {
    double confidenceIndex = 0.0;
    TargetValueType labelValue{m_DefaultLabel};
    if (inputMaskPtr)
      {
      validPoint = maskIt.Get() > 0;
      ++maskIt;
      }
    if (validPoint)
      {
      labelValue = labIt.GetMeasurementVector()[0];
      ++labIt;
      if(computeConfidenceMap)
        {
        confidenceIndex = confIt.GetMeasurementVector()[0];
        ++confIt;
        }
      }
    outIt.Set(labelValue);
    if (computeConfidenceMap)
      {
      confidenceIt.Set(confidenceIndex);
      ++confidenceIt;
      }
    }
  progress.CompletedPixel();
}
template <class TInputImage, class TOutputImage, class TMaskImage>
void
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  if(m_Batch)
    {
    std::cout << "Batch mode\n";
    this->BatchThreadedGenerateData(outputRegionForThread, threadId);
    }
  else
    {
    std::cout << "Classic mode\n";
    this->ClassicThreadedGenerateData(outputRegionForThread, threadId);
    }

}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, class TMaskImage>
void
SharkImageClassificationFilter<TInputImage, TOutputImage, TMaskImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
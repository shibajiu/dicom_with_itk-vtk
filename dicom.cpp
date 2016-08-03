#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageToVTKImageFilter.h"

#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImageActor.h"
#include "vtkImageViewer2.h"
#include "vtkImageFlip.h"
#include "vtkImageMapper3D.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"

void main(){
	typedef itk::Image<signed short, 2> imagetype;
	typedef itk::ImageFileReader<imagetype> imagereadertype;
	typedef itk::ImageToVTKImageFilter<imagetype> connerctortypr;

	imagereadertype::Pointer reader = imagereadertype::New();
	reader->SetFileName("e:/IM-0001-0001.dcm");
	auto iotype = itk::GDCMImageIO::New();
	reader->SetImageIO(iotype);

	try{
		reader->Update();
	}
	catch (itk::ExceptionObject e){
		std::cerr << "exception in file reader " << std::endl;
		std::cerr << e.GetDescription() << std::endl;
		std::cerr << e.GetLocation() << std::endl;
	}

	connerctortypr::Pointer connector = connerctortypr::New();
	connector->SetInput(reader->GetOutput());
	connector->Update();

	auto flip = vtkImageFlip::New();
	flip->SetInputData(connector->GetOutput());
	flip->SetFilteredAxis(1);
	flip->Update();

	auto actor = vtkImageActor::New();
	//actor->GetMapper()->SetInputData(flip->GetOutput());
	actor->SetInputData(flip->GetOutput());
	actor->InterpolateOff();
	actor->Update();

	auto render = vtkRenderer::New();
	render->AddActor(actor);

	auto window = vtkRenderWindow::New();
	window->SetSize(800, 600);
	window->AddRenderer(render);
	

	auto interactor = vtkRenderWindowInteractor::New();
	interactor->SetRenderWindow(window);

	interactor->Initialize();
	interactor->Start();
}
// Editor
#include <CodeAnalyze/BehaviourSearchVisitor.hpp>

class ExampleASTConsumer : public clang::ASTConsumer
{
public:
    // override the constructor in order to pass CI
    explicit ExampleASTConsumer(HG::CodeAnalyze::BehavioursSearchVisitor::Result* result) :
            visitor(new HG::CodeAnalyze::BehavioursSearchVisitor(result)) // initialize the visitor
    { }

    // override this to call our ExampleVisitor on the entire source file
    virtual void HandleTranslationUnit(clang::ASTContext &Context)
    {
        /* we can use ASTContext to get the TranslationUnitDecl, which is
             a single Decl that collectively represents the entire source file */
        visitor->TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    HG::CodeAnalyze::BehavioursSearchVisitor *visitor; // doesn't have to be private
};

class ExampleFrontendAction : public clang::ASTFrontendAction
{
public:
    explicit ExampleFrontendAction(HG::CodeAnalyze::BehavioursSearchVisitor::Result* result) :
            m_result(result)
    {

    }

protected:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override
    {
        return std::make_unique<ExampleASTConsumer>(m_result); // pass CI pointer to ASTConsumer
    }

private:
    HG::CodeAnalyze::BehavioursSearchVisitor::Result* m_result;
};

class ExampleActionFabric : public clang::tooling::FrontendActionFactory
{
public:

    explicit ExampleActionFabric(HG::CodeAnalyze::BehavioursSearchVisitor::Result* result) :
            m_result(result)
    {

    }

    clang::FrontendAction* create() override
    {
        return new ExampleFrontendAction(m_result);
    }

private:
    HG::CodeAnalyze::BehavioursSearchVisitor::Result* m_result;
};

HG::CodeAnalyze::BehavioursSearchVisitor::BehavioursSearchVisitor(Result* result) :
        m_result(result),
        m_behaviourDecl(nullptr)
{

}

bool HG::CodeAnalyze::BehavioursSearchVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl* declaration)
{
    if (declaration->getQualifiedNameAsString() == "HG::Core::Behaviour" &&
        declaration->isCompleteDefinition() &&
        m_behaviourDecl == nullptr)
    {
        m_behaviourDecl = declaration;
        return true;
    }

    if (declaration->getTypeForDecl()->isClassType() &&
        declaration->isCompleteDefinition() &&
        m_behaviourDecl)
    {
        if (declaration->getNumBases() > 0 &&
            declaration->isDerivedFrom(m_behaviourDecl))
        {
            // Saving name
            m_result->names.emplace_back(declaration->getQualifiedNameAsString());
        }
    }

    return true;
}

std::unique_ptr<clang::tooling::ToolAction> HG::CodeAnalyze::BehavioursSearchVisitor::createFabric(HG::CodeAnalyze::BehavioursSearchVisitor::Result* result)
{
    return std::make_unique<ExampleActionFabric>(result);
}
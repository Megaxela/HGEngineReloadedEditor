#pragma once
// C++ STL
#include <memory>

// clang
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Tooling/Tooling.h>

namespace HG::CodeAnalyze
{
    /**
     * @brief Class, that describes recursive AST visitor
     * for detecting behaviours.
     */
    class BehavioursSearchVisitor : public clang::RecursiveASTVisitor<BehavioursSearchVisitor>
    {
    public:

        struct Result
        {
            Result() :
                    success(false),
                    name()
            {

            }

            bool success;
            std::string name;
        };

        /**
         * @brief Constructor.
         */
        explicit BehavioursSearchVisitor(Result* result);

        /**
         * @brief Visitor method implementation.
         * @param declaration Pointer to declaration.
         */
        bool VisitCXXRecordDecl(clang::CXXRecordDecl* declaration);

        /**
         * @brief Method for creating tool action fabric
         * with this visitor.
         * @return Pointer to tool action.
         */
        static std::unique_ptr<clang::tooling::ToolAction> createFabric(Result* result);

    private:

        Result* m_result;
        clang::CXXRecordDecl* m_behaviourDecl;
    };
}
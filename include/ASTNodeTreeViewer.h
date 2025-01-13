#include "ASTNode.h"
#include <iostream>

namespace AST
{
	class ASTNodeTreeViewer
	{
	public:
		struct NodeTreeInfo
		{
			std::vector<std::string> mLines = {};
			std::vector<size_t> mLineContentBeginOffset = {};
			size_t mWidth = 0;

			void MergeRight(const NodeTreeInfo& rightNodeTreeInfo, const int32_t offset);

			size_t LineRemainingSpace(const size_t lineIndex) const;

			int32_t GetLeftPivot() const;
			int32_t GetMiddlePivot() const;
			int32_t GetRightPivot() const;

			int32_t GetLeftPivotRemainingSpace() const;
			int32_t GetMiddlePivotRemainingSpace() const;
			int32_t GetRightPivotRemainingSpace() const;

			std::string ToString() const;
		};

	private:
		static void UpdateLeafNodeTreeInfo(ASTNodeTreeViewer::NodeTreeInfo& outNodeTreeInfo, const std::string& symbol);

		static void FormOperatorSubtree(const OperatorNode* operatorNode, ASTNodeTreeViewer::NodeTreeInfo& outNodeTreeInfo);

		static void PrintTreeInternal(const ASTNode* node, NodeTreeInfo& outNodeTreeInfo);

	public:
		static std::string PrintTree(const ASTNode* node);
	};

	std::ostream& operator<<(std::ostream& os, const ASTNodeTreeViewer::NodeTreeInfo& nodeTreeInfo);

} // namespace AST
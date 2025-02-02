#include "ASTNodeTreeViewer.h"

namespace AST
{
	static const size_t kMinItemDistance = 2;

	void ASTNodeTreeViewer::NodeTreeInfo::MergeRight(const ASTNodeTreeViewer::NodeTreeInfo& rightNodeTreeInfo, const int32_t offset)
	{
		size_t rightNodeTreeInfoSize = rightNodeTreeInfo.mLines.size();
		size_t thisNodeTreeInfoSize = mLines.size();
		if (rightNodeTreeInfoSize > thisNodeTreeInfoSize)
		{
			mLines.resize(rightNodeTreeInfoSize);
			mLineContentBeginOffset.resize(rightNodeTreeInfoSize);
		}
		for (size_t i = 0; i < rightNodeTreeInfoSize; i++)
		{
			if (i < thisNodeTreeInfoSize)
			{
				mLines[i] += std::string(LineRemainingSpace(i) + rightNodeTreeInfo.mLineContentBeginOffset[i] + offset, ' ') + rightNodeTreeInfo.mLines[i];
			}
			else
			{
				mLines[i] = rightNodeTreeInfo.mLines[i];
				mLineContentBeginOffset[i] = mWidth + rightNodeTreeInfo.mLineContentBeginOffset[i] + offset;
			}
		}

		mWidth += rightNodeTreeInfo.mWidth + offset;
	}

	size_t ASTNodeTreeViewer::NodeTreeInfo::LineRemainingSpace(const size_t lineIndex) const
	{
		return mWidth - mLineContentBeginOffset[lineIndex] - mLines[lineIndex].size();
	}

	int32_t ASTNodeTreeViewer::NodeTreeInfo::GetLeftPivot() const
	{
		return mLines[0].size() == 1 ? static_cast<int32_t>(mLineContentBeginOffset[0]) + 1 : static_cast<int32_t>(mLineContentBeginOffset[0] + mLines[0].size()) - 1;
	}

	int32_t ASTNodeTreeViewer::NodeTreeInfo::GetMiddlePivot() const
	{
		return static_cast<int32_t>(mLineContentBeginOffset[0] + (mLines[0].size() - (mLines[0].size() + 1) % 2) / 2);
	}

	int32_t ASTNodeTreeViewer::NodeTreeInfo::GetRightPivot() const
	{
		return mLines[0].size() == 1 ? static_cast<int32_t>(mLineContentBeginOffset[0]) - 1 : static_cast<int32_t>(mLineContentBeginOffset[0]);
	}

	int32_t ASTNodeTreeViewer::NodeTreeInfo::GetLeftPivotRemainingSpace() const
	{
		return static_cast<int32_t>(mWidth) - GetLeftPivot() - 1; // -1 to account for the line character
	}

	int32_t ASTNodeTreeViewer::NodeTreeInfo::GetMiddlePivotRemainingSpace() const
	{
		return static_cast<int32_t>(mWidth) - GetMiddlePivot() - 1; // -1 to account for the line character
	}

	int32_t ASTNodeTreeViewer::NodeTreeInfo::GetRightPivotRemainingSpace() const
	{
		return static_cast<int32_t>(mWidth) - GetRightPivot() - 1; // -1 to account for the line character
	}

	std::string ASTNodeTreeViewer::NodeTreeInfo::ToString() const
	{
		std::string result;
		for (size_t i = 0; i < mLines.size(); i++)
		{
			result += std::string(mLineContentBeginOffset[i], ' ') + mLines[i];
			if (i < mLines.size() - 1) // Add newline only if it's not the last line
			{
				result += "\n";
			}
		}
		return result;
	}

	void ASTNodeTreeViewer::UpdateLeafNodeTreeInfo(ASTNodeTreeViewer::NodeTreeInfo& outNodeTreeInfo, const std::string& symbol)
	{
		size_t symbolSize = symbol.size();
		outNodeTreeInfo.mLines.push_back(symbol);
		outNodeTreeInfo.mLineContentBeginOffset.push_back(0);
		outNodeTreeInfo.mWidth = symbolSize;
	}

	struct LineIntervalInfo
	{
		size_t mLeftNodeIndex = 0;
		size_t mRightNodeIndex = 0;
		size_t mLeftOffset = 0;
		size_t mRightOffset = 0;
	};

	void ASTNodeTreeViewer::FormOperatorSubtree(const OperatorNode* operatorNode, ASTNodeTreeViewer::NodeTreeInfo& outNodeTreeInfo)
	{
		// corner case: if the operator node has no operands, use UpdateLeafNodeTreeInfo
		if (operatorNode->mOperands.empty())
		{
			UpdateLeafNodeTreeInfo(outNodeTreeInfo, operatorNode->mOperator->mSymbol);
			return;
		}

		// corner case: if the operator node has only one operand, then
		// 1. get the child node
		// 2. get the pivot point of the child node
		// 3. add 1 to the pivot point to get the offset of the operator symbol
		// 4. add the operator symbol to the line
		// 5. return
		if (operatorNode->mOperands.size() == 1)
		{
			const ASTNode* operand = operatorNode->mOperands[0];
			NodeTreeInfo childNodeTreeInfo = {};
			PrintTreeInternal(operand, childNodeTreeInfo);
			outNodeTreeInfo.mLines.reserve(childNodeTreeInfo.mLines.size() + 2);
			outNodeTreeInfo.mLineContentBeginOffset.reserve(childNodeTreeInfo.mLines.size() + 2);

			// add the operator symbol to the line
			outNodeTreeInfo.mLines.push_back(operatorNode->mOperator->mSymbol);
			outNodeTreeInfo.mLineContentBeginOffset.push_back(childNodeTreeInfo.GetLeftPivot() + 1);

			// add a line to connect the operator symbol and the child node
			outNodeTreeInfo.mLines.push_back("/");
			outNodeTreeInfo.mLineContentBeginOffset.push_back(childNodeTreeInfo.GetLeftPivot());

			size_t operatorSize = operatorNode->mOperator->mSymbol.size();
			if (operatorSize + outNodeTreeInfo.mLineContentBeginOffset[0] > outNodeTreeInfo.mWidth)
			{
				outNodeTreeInfo.mWidth = operatorSize + outNodeTreeInfo.mLineContentBeginOffset[0];
			}
			else
			{
				outNodeTreeInfo.mWidth = childNodeTreeInfo.mWidth;
			}

			// copy the child node's lines to the parent node
			outNodeTreeInfo.mLines.insert(outNodeTreeInfo.mLines.end(), childNodeTreeInfo.mLines.begin(), childNodeTreeInfo.mLines.end());
			outNodeTreeInfo.mLineContentBeginOffset.insert(outNodeTreeInfo.mLineContentBeginOffset.end(), childNodeTreeInfo.mLineContentBeginOffset.begin(), childNodeTreeInfo.mLineContentBeginOffset.end());

			return;
		}

		// read all child nodes
		std::vector<NodeTreeInfo> childNodeTreeInfos = {};

		std::vector<size_t> pivotOffsets = {};
		std::vector<size_t> firstLineInterval = {};
		size_t maxFirstLineInterval = 0;
		std::vector<int32_t> lineOffsetAdjustions = {};
		size_t accumulativeWidth = 0;

		childNodeTreeInfos.reserve(operatorNode->mOperands.size());

		pivotOffsets.reserve(operatorNode->mOperands.size());
		firstLineInterval.reserve(operatorNode->mOperands.size() - 1);
		lineOffsetAdjustions.reserve(operatorNode->mOperands.size() - 1);

		for (size_t i = 0; i < operatorNode->mOperands.size(); i++)
		{
			// read child node
			const ASTNode* operand = operatorNode->mOperands[i];
			NodeTreeInfo childNodeTreeInfo = {};
			PrintTreeInternal(operand, childNodeTreeInfo);
			childNodeTreeInfos.push_back(childNodeTreeInfo);

			// store pivot points
			// pivot points are the offset of the beginning of the line between each child node and the parent node
			int32_t pivotOffset = 0;
			if (i == 0)
			{
				pivotOffset = childNodeTreeInfo.GetLeftPivot();

				// store the pivot point of the left-most child node
				// we can push it back directly here because we don't need to adjust it
				pivotOffsets.push_back(pivotOffset);
				accumulativeWidth += childNodeTreeInfo.mWidth;
			}
			else if (i == operatorNode->mOperands.size() - 1)
			{
				pivotOffset = childNodeTreeInfo.GetRightPivot();
			}
			else
			{
				pivotOffset = childNodeTreeInfo.GetMiddlePivot();
			}

			// calculate the first round of line offset adjustments
			// 1. get the remaining space of the last character of the left node
			//    do so by calling LineRemainingSpace on the last line of the left node
			// then add the mLineContentBeginOffset of the right node
			// this way we get the distance between the last character of the left node and the first character of the right node before any adjustments
			// we need to keep in mind, we need the minimum distance between the last character of the left node and the first character of the right node to be at least kMinItemDistance
			// but, we also need to minimize the minimum distance between the last character of the left node and the first character of the right node
			// so, loop through all lines of the left node and right node, and calculate the minimum distance between the last character of the left node and the first character of the right node
			// then, calculate the difference between the minimum distance and kMinItemDistance
			// this difference is the offset adjustment
			if (i != 0)
			{
				size_t leftNodeLastLineRemainingSpace = childNodeTreeInfos[i - 1].LineRemainingSpace(0);
				size_t rightNodeOffset = childNodeTreeInfos[i].mLineContentBeginOffset[0];
				firstLineInterval.push_back(leftNodeLastLineRemainingSpace + rightNodeOffset);
				size_t minInterLineOffset = firstLineInterval[i - 1];

				int32_t prevNodeIndex = static_cast<int32_t>(i) - 1;
				size_t curLineIndex = 1;
				size_t inBetweenWidth = 0;

				while (prevNodeIndex >= 0 && curLineIndex < childNodeTreeInfos[i].mLines.size())
				{
					for (; curLineIndex < std::min(childNodeTreeInfos[prevNodeIndex].mLines.size(), childNodeTreeInfos[i].mLines.size()); curLineIndex++)
					{
						leftNodeLastLineRemainingSpace = childNodeTreeInfos[prevNodeIndex].LineRemainingSpace(curLineIndex);
						rightNodeOffset = childNodeTreeInfos[i].mLineContentBeginOffset[curLineIndex];

						minInterLineOffset = std::min(minInterLineOffset, leftNodeLastLineRemainingSpace + rightNodeOffset + inBetweenWidth);
					}
					inBetweenWidth += childNodeTreeInfos[prevNodeIndex].mWidth;
					prevNodeIndex--;
				}

				int32_t proposedAdjustion = static_cast<int32_t>(kMinItemDistance) - static_cast<int32_t>(minInterLineOffset);

				// left-shift must be less than the width of the total content of the left node, otherwise the index will be negative
				if (proposedAdjustion < 0 && -proposedAdjustion > accumulativeWidth)
				{
					lineOffsetAdjustions.push_back(-static_cast<int32_t>(accumulativeWidth));
				}
				else
				{
					lineOffsetAdjustions.push_back(proposedAdjustion);
				}

				firstLineInterval[i - 1] = firstLineInterval[i - 1] + lineOffsetAdjustions[i - 1];
				maxFirstLineInterval = std::max(maxFirstLineInterval, firstLineInterval[i - 1]);

				// update the pivot line interval
				pivotOffsets.push_back(accumulativeWidth + pivotOffset + lineOffsetAdjustions[i - 1]);

				accumulativeWidth += childNodeTreeInfos[i].mWidth + lineOffsetAdjustions[i - 1];
			}
		}

		// balance the first line interval
		// doing so by updating the line offset adjustments and the first line interval with the difference between the maximum first line interval and the current first line interval
		// we can also store the width of the first line's content to compare with the size of the operator symbol
		size_t accumulativeOffset = 0;
		for (size_t i = 0; i < lineOffsetAdjustions.size(); i++)
		{
			size_t firstLineBalancingOffset = maxFirstLineInterval - firstLineInterval[i];
			lineOffsetAdjustions[i] += static_cast<int32_t>(firstLineBalancingOffset);
			accumulativeOffset += firstLineBalancingOffset;
			pivotOffsets[i + 1] += accumulativeOffset;
		}
		// we don't need firstLineInterval anymore after this point

		// compare the size of the operator symbol with the size of the first line content
		// if the first line content is smaller than the operator symbol, add evenly distributed spaces to the line offset adjustments
		// also, in this case, we know that the line height is 1
		size_t lineHeight = 0;
		size_t pivotLineContentWidth = pivotOffsets.back() - pivotOffsets.front() - 1;
		if (pivotLineContentWidth < operatorNode->mOperator->mSymbol.size())
		{
			size_t additionalSpaceDivided = (operatorNode->mOperator->mSymbol.size() - pivotLineContentWidth) / lineOffsetAdjustions.size();
			size_t additionalSpaceRemainder = (operatorNode->mOperator->mSymbol.size() - pivotLineContentWidth) % lineOffsetAdjustions.size();
			for (size_t i = 0; i < lineOffsetAdjustions.size(); i++)
			{
				lineOffsetAdjustions[i] += static_cast<int32_t>(additionalSpaceDivided);
				pivotOffsets[i + 1] += additionalSpaceDivided * (i + 1);
				if (i < additionalSpaceRemainder)
				{
					lineOffsetAdjustions[i] += 1;

					for (size_t j = i + 1; j < pivotOffsets.size(); j++)
					{
						pivotOffsets[j]++;
					}
				}
			}
			lineHeight = 1;
		}
		else
		{
			size_t firstLineContentWidthDifference = pivotLineContentWidth - operatorNode->mOperator->mSymbol.size();

			// if the first line content is larger than the operator symbol, but
			// the difference between the first line content and the operator symbol is odd
			// add 1 to the last line offset adjustment
			if (firstLineContentWidthDifference % 2)
			{
				firstLineContentWidthDifference++;
				lineOffsetAdjustions[lineOffsetAdjustions.size() - 1]++;
				// update the pivot line intervals
				// we don't update the first pivot line interval because is the left-most child node
				pivotOffsets[pivotOffsets.size() - 1]++;
			}

			// calculate the line height
			lineHeight = firstLineContentWidthDifference / 2 + 1;
		}

		// 1. resize the lines and line content begin offset vectors to the final height
		outNodeTreeInfo.mLines.resize(lineHeight + 1);
		outNodeTreeInfo.mLineContentBeginOffset.resize(lineHeight + 1);

		// 2. add the operator symbol to the line
		outNodeTreeInfo.mLines[0] = operatorNode->mOperator->mSymbol;
		outNodeTreeInfo.mLineContentBeginOffset[0] = childNodeTreeInfos[0].GetLeftPivot() + lineHeight;

		// 3. add the line to connect the operator symbol and the child node
		size_t leftNodeClosestLine = 1;
		size_t rightNodeClosestLine = pivotOffsets.size() - 2;
		for (size_t i = lineHeight; i > 0; i--)
		{
			for (size_t j = 0; j < operatorNode->mOperands.size(); j++)
			{
				if (j == 0)
				{
					outNodeTreeInfo.mLines[i] += "/";
					outNodeTreeInfo.mLineContentBeginOffset[i] = outNodeTreeInfo.mLineContentBeginOffset[0] - i;
					if (pivotOffsets[leftNodeClosestLine] <= pivotOffsets[j])
					{
						leftNodeClosestLine++;
						if (leftNodeClosestLine > rightNodeClosestLine ||
							(leftNodeClosestLine == rightNodeClosestLine && pivotOffsets[pivotOffsets.size() - 1] - pivotOffsets[rightNodeClosestLine] == 0))
						{
							leftNodeClosestLine = operatorNode->mOperands.size() - 1;
						}
					}
					pivotOffsets[j]++;
				}
				else if (j == operatorNode->mOperands.size() - 1)
				{
					int32_t isNotLeftNode = j != leftNodeClosestLine && rightNodeClosestLine != 0;
					size_t spaceToFill = static_cast<int32_t>(pivotOffsets[j] - pivotOffsets[rightNodeClosestLine] - isNotLeftNode);
					outNodeTreeInfo.mLines[i] += std::string(spaceToFill, ' ') + "\\";
					if (pivotOffsets[j] - pivotOffsets[rightNodeClosestLine] <= 1)
					{
						rightNodeClosestLine--;
						if (rightNodeClosestLine < leftNodeClosestLine)
						{
							rightNodeClosestLine = 0;
						}
					}
					pivotOffsets[j]--;
				}
				else if (j == leftNodeClosestLine && leftNodeClosestLine <= rightNodeClosestLine)
				{
					outNodeTreeInfo.mLines[i] += std::string(pivotOffsets[j] - pivotOffsets[0], ' ') + "|";
				}
				else if (j >= leftNodeClosestLine && j <= rightNodeClosestLine) // j already not leftNodeClosestLine, thus j - 1 definitely exists
				{
					outNodeTreeInfo.mLines[i] += std::string(pivotOffsets[j] - pivotOffsets[j - 1] - 1, ' ') + "|";
				}
			}
		}

		// 4. merge all child nodes to the first child node
		for (size_t i = 1; i < childNodeTreeInfos.size(); i++)
		{
			childNodeTreeInfos[0].MergeRight(childNodeTreeInfos[i], lineOffsetAdjustions[i - 1]);
		}

		// 5. copy the child node's lines to the parent node
		outNodeTreeInfo.mLines.insert(outNodeTreeInfo.mLines.end(), childNodeTreeInfos[0].mLines.begin(), childNodeTreeInfos[0].mLines.end());
		outNodeTreeInfo.mLineContentBeginOffset.insert(outNodeTreeInfo.mLineContentBeginOffset.end(), childNodeTreeInfos[0].mLineContentBeginOffset.begin(), childNodeTreeInfos[0].mLineContentBeginOffset.end());
		outNodeTreeInfo.mWidth = childNodeTreeInfos[0].mWidth;
	}

	void ASTNodeTreeViewer::PrintTreeInternal(const ASTNode* node, NodeTreeInfo& outNodeTreeInfo)
	{
		if (!node) return;

		switch (node->mType)
		{
		case ASTNode::NodeType::Rational:
		{
			const RationalNode* rationalNode = dynamic_cast<const RationalNode*>(node);
			UpdateLeafNodeTreeInfo(outNodeTreeInfo, rationalNode->mValue.ToString());
			break;
		}
		case ASTNode::NodeType::Irrational:
		{
			const IrrationalNode* irrationalNode = dynamic_cast<const IrrationalNode*>(node);
			UpdateLeafNodeTreeInfo(outNodeTreeInfo, irrationalNode->mIrrational->mSymbol);
			break;
		}
		case ASTNode::NodeType::Variable:
		{
			const VariableNode* variableNode = dynamic_cast<const VariableNode*>(node);
			UpdateLeafNodeTreeInfo(outNodeTreeInfo, variableNode->mVariable->mSymbol);
			break;
		}
		case ASTNode::NodeType::Parenthesis:
		{
			const ParenthesisNode* parenthesisNode = dynamic_cast<const ParenthesisNode*>(node);
			UpdateLeafNodeTreeInfo(outNodeTreeInfo, parenthesisNode->mParenthesis->mSymbol);
			break;
		}
		case ASTNode::NodeType::Operator:
		{
			const OperatorNode* operatorNode = dynamic_cast<const OperatorNode*>(node);
			outNodeTreeInfo = {};
			FormOperatorSubtree(operatorNode, outNodeTreeInfo);
			break;
		}
		default:
			break;
		}
	}

	std::string ASTNodeTreeViewer::PrintTree(const ASTNode* node)
	{
		NodeTreeInfo nodeTreeInfo = {};
		PrintTreeInternal(node, nodeTreeInfo);

		return nodeTreeInfo.ToString();
	}

	std::ostream& operator<<(std::ostream& os, const ASTNodeTreeViewer::NodeTreeInfo& nodeTreeInfo)
	{
		for (size_t i = 0; i < nodeTreeInfo.mLines.size(); i++)
		{
			os << std::string(nodeTreeInfo.mLineContentBeginOffset[i], ' ') << nodeTreeInfo.mLines[i] << std::endl;
		}
		return os;
	}
}
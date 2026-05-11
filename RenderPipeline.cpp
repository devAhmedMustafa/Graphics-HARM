#include "RenderPipeline.h"

RenderPipeline* RenderPipeline::m_Instance = nullptr;

std::optional<RenderCommand> RenderPipeline::getRenderCommand() const {

	if (heldCommand.has_value()) return heldCommand;

	if (m_Context.channel.empty()) return std::nullopt;

	auto early = m_Context.channel.front();
	m_Context.channel.pop();

	return early;
}
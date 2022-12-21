library(readr)
library(tidyr)
library(dplyr)
library(ggplot2)
library(RColorBrewer)

# importing csv
p <- read_csv("./profiling.csv")

# avg and std
p_avg_std <- p %>%
  group_by(n, a) %>%
  summarise(avg = mean(t),
            std = sd(t))

# linear plot
p_lin <- ggplot(data = p_avg_std, aes(x = n, y = avg, color = a)) +
  geom_point() +
  geom_line() +
  geom_errorbar(aes(ymin = avg - std, ymax = avg + std), width = 0.1, linewidth = 0.5) +
  geom_smooth(method = "loess", linewidth = 0.25) +
  scale_x_continuous(breaks = scales::pretty_breaks(n = 10), labels = ~ format(.x, scientific = FALSE)) +
  scale_y_continuous(breaks = scales::pretty_breaks(n = 10), labels = ~ format(.x, scientific = TRUE)) +
  labs(
    title = "Variazione del tempo di esecuzione in funzione della dimensione dell'input",
    subtitle = "Scala lineare",
    caption = "10 iterazioni per campione",
    x = "Input size",
    y = "Execution time"
  ) +
  scale_color_brewer(palette="Dark2") +
  theme(legend.position = c(.10, .90), legend.background = element_rect(fill = "transparent"), legend.title=element_blank())

p_lin

ggsave("./p_lin.png", width = 7.5)

# 2x log plot
p_log <- ggplot(data = p_avg_std, aes(x = n, y = avg, color = a)) +
  geom_point() +
  geom_line() +
  geom_smooth(method = "loess", linewidth = 0.25) +
  scale_y_continuous(trans='log10') +
  scale_x_continuous(trans='log10', labels = ~ format(.x, scientific = FALSE)) +
  labs(
    title = "Variazione del tempo di esecuzione in funzione della dimensione dell'input",
    subtitle = "Scala logaritmica",
    caption = "10 iterazioni per campione",
    x = "Input size",
    y = "Execution time"
  ) +
  scale_color_brewer(palette="Dark2") +
  theme(legend.position = c(.10, .90), legend.background = element_rect(fill = "transparent"), legend.title=element_blank())

p_log

ggsave("./p_log.png", width = 7.5)
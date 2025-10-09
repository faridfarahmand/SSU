


%% Scope data (time base)
t_scope_start = -2.00425;
t_scope_end   = 0.435;
dt_scope      = 25e-5;
t_shift       = -1.3825;  % <-- WHEN THE SIGNAL START RISING

V             = 1.847;       % pulse amplitude (V)
% Load measured channel (2nd column)
data    = readtable('OneCycle3.csv');
%-----------------
t_scope = (t_scope_start:dt_scope:t_scope_end)';   % column vector
y_scope = data{:,2};

% Trim lengths if needed
N = min(length(t_scope), length(y_scope));
t_scope = t_scope(1:N);
y_scope = y_scope(1:N);

%% Simulation parameters
tau = 0.1;     % RC (s)
V   = 1.847;       % pulse amplitude (V)
t0  = 0.0;     % pulse start time (s) in the SIMULATED time frame
Tp  = 1.0;     % pulse width (s)

% Use a simulated time axis that starts at zero (same length as scope)
t_sim = t_scope - t_scope_start;
% Unit step

u = @(x) double(x >= 0);

% RC response to rectangular pulse of width Tp starting at t0:

y_sim = V * ( ...

    (1 - exp(-(t_sim - t0)./tau)) .* u(t_sim - t0) ...                              % rise

  - ( (1 - exp(-(t_sim - t0 - Tp)./tau)).* u(t_sim - t0 - Tp) ) ... % decay

);


%% Alignment controls (adjust to make waveforms coincide)
% Shift the scope time so its key edge aligns with the simulated t=0 reference.
t_scope_aligned = t_scope - t_shift;        % measured trace time after alignment


% Optional vertical scale/offset for scope (if needed)
gain   = 1.0;   % multiply scope amplitude
offset = 0.0;   % add DC offset to scope
y_scope_adj = gain * y_scope + offset;


%% ---- Overlay plot ----
figure; hold on; grid on;
plot(t_sim, y_sim, 'r', 'LineWidth', 1.8);                 % simulated
plot(t_scope_aligned, y_scope_adj, 'b', 'LineWidth', 1.4);  % measured (aligned)

% Mark pulse start/end of the simulated signal
xline(t0,   'k:');
xline(t0+Tp,'k:');
xlabel('Time (s)');
ylabel('Voltage (V)');
title(sprintf('Simulated vs Measured (\\tau = %.3g s, T_p = %.3g s)', tau, Tp));
legend('Simulated', 'Measured (aligned)', 'Location', 'best');

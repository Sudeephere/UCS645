let performanceChart = null;

document.getElementById('benchmark-form').addEventListener('submit', async (e) => {
    e.preventDefault();
    
    const size = document.getElementById('array-size').value;
    const btn = document.getElementById('run-btn');
    const btnText = btn.querySelector('.btn-text');
    const btnLoader = document.getElementById('btn-loader');
    const resultsSection = document.getElementById('results-section');
    const errorMsg = document.getElementById('error-msg');

    // UI Loading State
    btn.disabled = true;
    btnText.classList.add('hidden');
    btnLoader.classList.remove('hidden');
    errorMsg.classList.add('hidden');

    try {
        const response = await fetch(`/api/benchmark?size=${size}`);
        const data = await response.json();

        if (!response.ok) {
            throw new Error(data.error || 'Failed to benchmark');
        }

        // Update UI Metrics
        document.getElementById('serial-time').textContent = `${data.serial_ms.toFixed(2)} ms`;
        document.getElementById('parallel-time').textContent = `${data.parallel_ms.toFixed(2)} ms`;
        document.getElementById('adaptive-time').textContent = `${data.adaptive_ms.toFixed(2)} ms`;
        
        document.getElementById('speedup-val').textContent = `${data.speedup}x`;
        document.getElementById('thread-info').textContent = `Max OpenMP Threads: ${data.threads}`;

        // Show Results
        resultsSection.classList.remove('hidden');

        // Update Chart
        updateChart(data.serial_ms, data.parallel_ms, data.adaptive_ms);

    } catch (error) {
        errorMsg.textContent = error.message;
        errorMsg.classList.remove('hidden');
        resultsSection.classList.add('hidden');
    } finally {
        // Reset UI State
        btn.disabled = false;
        btnText.classList.remove('hidden');
        btnLoader.classList.add('hidden');
    }
});

function updateChart(serial, parallel, adaptive) {
    const ctx = document.getElementById('performanceChart').getContext('2d');
    
    if (performanceChart) {
        performanceChart.destroy();
    }

    // Chart.js defaults for dark theme
    Chart.defaults.color = '#94a3b8';
    Chart.defaults.font.family = "'Inter', sans-serif";

    performanceChart = new Chart(ctx, {
        type: 'bar',
        data: {
            labels: ['Serial Sort', 'Naive Parallel Sort', 'Adaptive Sort'],
            datasets: [{
                label: 'Execution Time (ms)',
                data: [serial, parallel, adaptive],
                backgroundColor: [
                    'rgba(239, 68, 68, 0.8)',   // Red
                    'rgba(245, 158, 11, 0.8)',  // Yellow
                    'rgba(16, 185, 129, 0.8)'   // Green
                ],
                borderColor: [
                    'rgb(239, 68, 68)',
                    'rgb(245, 158, 11)',
                    'rgb(16, 185, 129)'
                ],
                borderWidth: 1,
                borderRadius: 4
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            plugins: {
                legend: {
                    display: false
                },
                tooltip: {
                    callbacks: {
                        label: function(context) {
                            return context.parsed.y.toFixed(2) + ' ms';
                        }
                    }
                }
            },
            scales: {
                y: {
                    beginAtZero: true,
                    title: {
                        display: true,
                        text: 'Time (milliseconds)',
                        color: '#94a3b8'
                    },
                    grid: {
                        color: 'rgba(255, 255, 255, 0.05)'
                    }
                },
                x: {
                    grid: {
                        display: false
                    }
                }
            }
        }
    });
}
